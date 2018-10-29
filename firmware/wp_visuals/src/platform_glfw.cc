#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <err.h>

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

#include "gl.h"
#include "platform.h"
#include "sl.h"

namespace platform {

struct Internal {
  GLFWwindow *window = nullptr;
};

//////////////////////////////////////////////////////////////////////////////

static Key key_of_glfw_key(int gk) {
  // clang-format off
  switch (gk) {
    case GLFW_KEY_ESCAPE:           return Key::escape;
    case GLFW_KEY_F1:               return Key::f1;
    case GLFW_KEY_F2:               return Key::f2;
    case GLFW_KEY_F3:               return Key::f3;
    case GLFW_KEY_F4:               return Key::f4;
    case GLFW_KEY_F5:               return Key::f5;
    case GLFW_KEY_F6:               return Key::f6;
    case GLFW_KEY_F7:               return Key::f7;
    case GLFW_KEY_F8:               return Key::f8;
    case GLFW_KEY_F9:               return Key::f9;
    case GLFW_KEY_F10:              return Key::f10;
    case GLFW_KEY_F11:              return Key::f11;
    case GLFW_KEY_F12:              return Key::f12;
    case GLFW_KEY_PRINT_SCREEN:     return Key::print_screen;
    case GLFW_KEY_SCROLL_LOCK:      return Key::scroll_lock;
    case GLFW_KEY_PAUSE:            return Key::pause_break;

    case GLFW_KEY_GRAVE_ACCENT:     return Key::backtick;
    case GLFW_KEY_1:                return Key::one;
    case GLFW_KEY_2:                return Key::two;
    case GLFW_KEY_3:                return Key::three;
    case GLFW_KEY_4:                return Key::four;
    case GLFW_KEY_5:                return Key::five;
    case GLFW_KEY_6:                return Key::six;
    case GLFW_KEY_7:                return Key::seven;
    case GLFW_KEY_8:                return Key::eight;
    case GLFW_KEY_9:                return Key::nine;
    case GLFW_KEY_0:                return Key::zero;
    case GLFW_KEY_MINUS:            return Key::minus;
    case GLFW_KEY_EQUAL:            return Key::equals;
    case GLFW_KEY_BACKSPACE:        return Key::backspace;
    case GLFW_KEY_INSERT:           return Key::insert;
    case GLFW_KEY_HOME:             return Key::home;
    case GLFW_KEY_PAGE_UP:          return Key::page_up;

    case GLFW_KEY_TAB:              return Key::tab;
    case GLFW_KEY_Q:                return Key::q;
    case GLFW_KEY_W:                return Key::w;
    case GLFW_KEY_E:                return Key::e;
    case GLFW_KEY_R:                return Key::r;
    case GLFW_KEY_T:                return Key::t;
    case GLFW_KEY_Y:                return Key::y;
    case GLFW_KEY_U:                return Key::u;
    case GLFW_KEY_I:                return Key::i;
    case GLFW_KEY_O:                return Key::o;
    case GLFW_KEY_P:                return Key::p;
    case GLFW_KEY_LEFT_BRACKET:     return Key::bracket_left;
    case GLFW_KEY_RIGHT_BRACKET:    return Key::bracket_right;
    case GLFW_KEY_BACKSLASH:        return Key::backslash;
    case GLFW_KEY_DELETE:           return Key::del;
    case GLFW_KEY_END:              return Key::end;
    case GLFW_KEY_PAGE_DOWN:        return Key::page_down;

    case GLFW_KEY_CAPS_LOCK:        return Key::caps_lock;
    case GLFW_KEY_A:                return Key::a;
    case GLFW_KEY_S:                return Key::s;
    case GLFW_KEY_D:                return Key::d;
    case GLFW_KEY_F:                return Key::f;
    case GLFW_KEY_G:                return Key::g;
    case GLFW_KEY_H:                return Key::h;
    case GLFW_KEY_J:                return Key::j;
    case GLFW_KEY_K:                return Key::k;
    case GLFW_KEY_L:                return Key::l;
    case GLFW_KEY_SEMICOLON:        return Key::semicolon;
    case GLFW_KEY_APOSTROPHE:       return Key::quote;
    case GLFW_KEY_ENTER:            return Key::carriage_return;

    case GLFW_KEY_LEFT_SHIFT:       return Key::shift_left;
    case GLFW_KEY_Z:                return Key::z;
    case GLFW_KEY_X:                return Key::x;
    case GLFW_KEY_C:                return Key::c;
    case GLFW_KEY_V:                return Key::v;
    case GLFW_KEY_B:                return Key::b;
    case GLFW_KEY_N:                return Key::n;
    case GLFW_KEY_M:                return Key::m;
    case GLFW_KEY_COMMA:            return Key::comma;
    case GLFW_KEY_PERIOD:           return Key::period;
    case GLFW_KEY_SLASH:            return Key::slash;
    case GLFW_KEY_RIGHT_SHIFT:      return Key::shift_right;
    case GLFW_KEY_UP:               return Key::up;

    case GLFW_KEY_LEFT_CONTROL:     return Key::control_left;
    case GLFW_KEY_LEFT_SUPER:       return Key::windows_left;
    case GLFW_KEY_LEFT_ALT:         return Key::alt;
    case GLFW_KEY_SPACE:            return Key::space;
    case GLFW_KEY_RIGHT_ALT:        return Key::alt_gr;
    case GLFW_KEY_RIGHT_SUPER:      return Key::windows_right;
    case GLFW_KEY_MENU:             return Key::menu;
    case GLFW_KEY_RIGHT_CONTROL:    return Key::control_right;
    case GLFW_KEY_LEFT:             return Key::left;
    case GLFW_KEY_DOWN:             return Key::down;
    case GLFW_KEY_RIGHT:            return Key::right;

    default:
      return Key::unknown;
  };
  // clang-format on
}

// GLFW callbacks  ///////////////////////////////////////////////////////////

static void glfw_error(int error, const char *description) {
  std::fprintf(stderr, "GLFW error: %d %s", error, description);
}

static void glfw_key(GLFWwindow *window, int key, int scancode, int action,
                     int mods) {
  (void)scancode;
  (void)mods;

  auto p = reinterpret_cast<Platform *>(glfwGetWindowUserPointer(window));

  if (GLFW_PRESS == action) {
    p->events.push_back({EventType::key_down, {{key_of_glfw_key(key)}}});
  } else if (GLFW_REPEAT == action) {
    p->events.push_back({EventType::key_repeat, {{key_of_glfw_key(key)}}});
  } else if (GLFW_RELEASE == action) {
    p->events.push_back({EventType::key_up, {{key_of_glfw_key(key)}}});
  }
}

static void glfw_window_size(GLFWwindow *window, int w, int h) {
  auto p = reinterpret_cast<Platform *>(glfwGetWindowUserPointer(window));

  p->viewport_size = sl::ivec2{w, h};

  glViewport(0, 0, w, h);
}

// Reload  ///////////////////////////////////////////////////////////////////

void reload_init(Platform *p) {
  {
    auto i = new Internal;
    p->internal = i;
  }

  glfwSetErrorCallback(glfw_error);

  if (0 == glfwInit()) {
    errx(EXIT_FAILURE, "cannot initialize glfw");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_SAMPLES, 8);
  // glfwWindowHint(GLFW_SRGB_CAPABLE, 0);

  p->viewport_size = sl::ivec2{666, 373};
  p->internal->window = glfwCreateWindow(1280, 800, "hello", nullptr, nullptr);
  if (nullptr == p->internal->window) {
    glfwTerminate();
    errx(EXIT_FAILURE, "cannot create window");
  }

  glfwMakeContextCurrent(p->internal->window);

  GLenum glew_status;
  if (GLEW_OK != (glew_status = glewInit())) {
    errx(EXIT_FAILURE, "glewInit: %s", glewGetErrorString(glew_status));
  }

  // NOTE(mastensg): opengl 3 core seems to require a VAO being bound.
  // opengl es 2 doesn't support VAOs at all.
  // - https://www.khronos.org/bugzilla/show_bug.cgi?id=1508
  {
    gl::Array a;
    glGenVertexArrays(1, &a);
    glBindVertexArray(a);
  }
}

void reload_begin(Platform *p) {
  glDebugMessageCallback(gl::debug_callback, nullptr);
  glEnable(GL_DEBUG_OUTPUT);

  glfwSetWindowUserPointer(p->internal->window, p);

  glfwSetErrorCallback(glfw_error);
  glfwSetKeyCallback(p->internal->window, glfw_key);
  glfwSetWindowSizeCallback(p->internal->window, glfw_window_size);
}

bool reload_end(Platform *p) {
  return GLFW_TRUE == glfwWindowShouldClose(p->internal->window);
}

// Frame  ////////////////////////////////////////////////////////////////////

void frame_begin(Platform *p) {
  p->events.clear();
  glfwPollEvents();
}

bool frame_end(Platform *p) {
  glfwSwapBuffers(p->internal->window);

  return GLFW_TRUE == glfwWindowShouldClose(p->internal->window);
}

//////////////////////////////////////////////////////////////////////////////

void exit(Platform *p, int status) {
  (void)p;

  glfwTerminate();
  std::exit(status);
}

double time(Platform *p) {
  (void)p;

  return glfwGetTime();
}

//////////////////////////////////////////////////////////////////////////////

}  // namespace platform
