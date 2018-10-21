#include <chrono>
#include <cstdio>
#include <cstdlib>

#include <err.h>

#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>
#include <bcm_host.h>

#include "imgui/imgui.h"
#include "platform.h"

namespace platform {

struct Internal {
  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  std::chrono::time_point<std::chrono::high_resolution_clock> started_at;
};

// VC  ///////////////////////////////////////////////////////////////////////

static NativeWindowType vc_init(uint32_t *width, uint32_t *height) {
  VC_RECT_T dst_rect, src_rect;
  DISPMANX_DISPLAY_HANDLE_T dispman_display;
  DISPMANX_UPDATE_HANDLE_T dispman_update;
  DISPMANX_ELEMENT_HANDLE_T dispman_element;

  static EGL_DISPMANX_WINDOW_T native_window;

  bcm_host_init();

  if (!(*width || *height)) {
    if (-1 == graphics_get_display_size(0, width, height)) {
      err(EXIT_FAILURE, "graphics_get_display_size");
    }
  }

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width = *width;
  dst_rect.height = *height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = *width << 16;
  src_rect.height = *height << 16;

  dispman_display = vc_dispmanx_display_open(0);
  dispman_update = vc_dispmanx_update_start(0);
  dispman_element = vc_dispmanx_element_add(
      dispman_update, dispman_display, 0, &dst_rect, 0, &src_rect,
      DISPMANX_PROTECTION_NONE, 0, 0, DISPMANX_NO_ROTATE);

  native_window.element = dispman_element;
  native_window.width = *width;
  native_window.height = *height;

  vc_dispmanx_update_submit_sync(dispman_update);

  return &native_window;
}

// EGL  //////////////////////////////////////////////////////////////////////

static const char *egl_strerror(EGLint error) {
  switch (error) {
    case EGL_SUCCESS:
      return "EGL_SUCCESS, The last function succeeded without error.";
      break;
    case EGL_NOT_INITIALIZED:
      return "EGL_NOT_INITIALIZED, EGL is not initialized, or could not be "
             "initialized, for the specified EGL display connection.";
      break;
    case EGL_BAD_ACCESS:
      return "EGL_BAD_ACCESS, EGL cannot access a requested resource (for "
             "example a context is bound in another thread).";
      break;
    case EGL_BAD_ALLOC:
      return "EGL_BAD_ALLOC, EGL failed to allocate resources for the "
             "requested operation.";
      break;
    case EGL_BAD_ATTRIBUTE:
      return "EGL_BAD_ATTRIBUTE, An unrecognized attribute or attribute value "
             "was passed in the attribute list.";
      break;
    case EGL_BAD_CONTEXT:
      return "EGL_BAD_CONTEXT, An EGLContext argument does not name a valid "
             "EGL rendering context.";
      break;
    case EGL_BAD_CONFIG:
      return "EGL_BAD_CONFIG, An EGLConfig argument does not name a valid EGL "
             "frame buffer configuration.";
      break;
    case EGL_BAD_CURRENT_SURFACE:
      return "EGL_BAD_CURRENT_SURFACE, The current surface of the calling "
             "thread is a window, pixel buffer or pixmap that is no longer "
             "valid.";
      break;
    case EGL_BAD_DISPLAY:
      return "EGL_BAD_DISPLAY, An EGLDisplay argument does not name a valid "
             "EGL display connection.";
      break;
    case EGL_BAD_SURFACE:
      return "EGL_BAD_SURFACE, An EGLSurface argument does not name a valid "
             "surface (window, pixel buffer or pixmap) configured for GL "
             "rendering.";
      break;
    case EGL_BAD_MATCH:
      return "EGL_BAD_MATCH, Arguments are inconsistent (for example, a valid "
             "context requires buffers not supplied by a valid surface).";
      break;
    case EGL_BAD_PARAMETER:
      return "EGL_BAD_PARAMETER, One or more argument values are invalid.";
      break;
    case EGL_BAD_NATIVE_PIXMAP:
      return "EGL_BAD_NATIVE_PIXMAP, A NativePixmapType argument does not "
             "refer to a valid native pixmap.";
      break;
    case EGL_BAD_NATIVE_WINDOW:
      return "EGL_BAD_NATIVE_WINDOW, A NativeWindowType argument does not "
             "refer to a valid native window.";
      break;
    case EGL_CONTEXT_LOST:
      return "EGL_CONTEXT_LOST, A power management event has occurred. The "
             "application must destroy all contexts and reinitialise OpenGL ES "
             "state and objects to continue rendering.";
      break;
    default:
      return "Unknown EGL error.";
  }
}

static void egl_init(Platform *p, NativeWindowType native_window) {
  static const EGLint config_attrib_list[] = {EGL_RED_SIZE,   8,  //
                                              EGL_GREEN_SIZE, 8,  //
                                              EGL_BLUE_SIZE,  8,  //
                                              EGL_ALPHA_SIZE, 8,  //
                                              EGL_DEPTH_SIZE, 16,  //
                                              EGL_NONE};
  static const EGLint context_attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2,
                                               EGL_NONE};
  EGLConfig config;
  EGLint num_config;

  if (EGL_DEFAULT_DISPLAY ==
      (p->internal->display = eglGetDisplay(EGL_DEFAULT_DISPLAY))) {
    errx(EXIT_FAILURE, "eglGetDisplay: EGL_NO_DISPLAY, can't obtain display");
  }

  if (EGL_FALSE == eglInitialize(p->internal->display, NULL, NULL)) {
    errx(EXIT_FAILURE, "eglInitialize: %s", egl_strerror(eglGetError()));
  }

  if (EGL_FALSE == eglChooseConfig(p->internal->display, config_attrib_list,
                                   &config, 1, &num_config)) {
    errx(EXIT_FAILURE, "eglChooseConfig: %s", egl_strerror(eglGetError()));
  }

  if (EGL_FALSE == eglBindAPI(EGL_OPENGL_ES_API)) {
    errx(EXIT_FAILURE, "eglBindAPI: %s", egl_strerror(eglGetError()));
  }

  if (EGL_NO_CONTEXT == (p->internal->context = eglCreateContext(
                             p->internal->display, config, EGL_NO_CONTEXT,
                             context_attrib_list))) {
    errx(EXIT_FAILURE, "eglCreateContext: %s", egl_strerror(eglGetError()));
  }

  if (EGL_NO_SURFACE ==
      (p->internal->surface = eglCreateWindowSurface(
           p->internal->display, config, native_window, NULL))) {
    errx(EXIT_FAILURE, "eglCreateWindowSurface: %s",
         egl_strerror(eglGetError()));
  }

  if (EGL_FALSE == eglMakeCurrent(p->internal->display, p->internal->surface,
                                  p->internal->surface, p->internal->context))
    errx(EXIT_FAILURE, "eglMakeCurrent: %s", egl_strerror(eglGetError()));
}

// Reload  ///////////////////////////////////////////////////////////////////

void reload_init(Platform *p) {
  {
    auto i = new Internal;
    p->internal = i;
  }

  p->internal->started_at = std::chrono::high_resolution_clock::now();

  NativeWindowType window;
  unsigned w = 0, h = 0;
  window = vc_init(&w, &h);
  p->viewport_size = sl::ivec2{w, h};
  std::fprintf(stderr, "size: %u x %u\n", w, h);

  egl_init(p, window);

  glViewport(0, 0, w, h);

  // imgui
  {
    ImGuiIO &io = ImGui::GetIO();
    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
  }
}

void reload_begin(Platform *p) {
  (void)p;

  // imgui
  ImGuiIO &io = ImGui::GetIO();

  io.RenderDrawListsFn = nullptr;
  io.GetClipboardTextFn = nullptr;
  io.SetClipboardTextFn = nullptr;
  io.ClipboardUserData = nullptr;
}

bool reload_end(Platform *p) {
  (void)p;

  return false;
}

// Frame  ////////////////////////////////////////////////////////////////////

void frame_begin(Platform *p) {
  (void)p;

  // imgui
  {
    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = ImVec2(0.0f, 0.0f);
    ImGui::NewFrame();
  }
}

bool frame_end(Platform *p) {
  ImGui::EndFrame();

  eglSwapBuffers(p->internal->display, p->internal->surface);

  return false;
}

//////////////////////////////////////////////////////////////////////////////

void exit(Platform *p, int status) {
  eglMakeCurrent(p->internal->display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                 EGL_NO_CONTEXT);
  eglDestroySurface(p->internal->display, p->internal->surface);
  eglDestroyContext(p->internal->display, p->internal->context);
  eglTerminate(p->internal->display);
  std::exit(status);
}

double time(Platform *p) {
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> since_start = now - p->internal->started_at;
  return since_start.count();
}

}  // namespace platform
