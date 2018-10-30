#ifdef HAVE_CONFIG_H
#include "config.h"  // IWYU pragma: keep
#endif

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <random>
#include <thread>
#include <vector>

#include <err.h>
#include <time.h>

#include <cairo/cairo-ft.h>
#include <cairo/cairo.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <zmq.h>

#include "gl.h"
#include "platform.h"
#include "reload_main.h"
#include "sl.h"

namespace temp {
static const std::vector<float> vertices = {
    -1.0f, -1.0f, +0.0f, +1.0f,  // A
    +1.0f, -1.0f, +1.0f, +1.0f,  // B
    +1.0f, +1.0f, +1.0f, +0.0f,  // C
    -1.0f, -1.0f, +0.0f, +1.0f,  // A
    +1.0f, +1.0f, +1.0f, +0.0f,  // C
    -1.0f, +1.0f, +0.0f, +0.0f,  // D
};

static const char vs[] = R"___(
uniform sampler2D u_texture;

attribute vec2 a_position;
attribute vec2 a_texpos;

varying vec2 f_texpos;

void main() {
  f_texpos = a_texpos;
  gl_Position = vec4(a_position, 0.0, 1.0);
}
)___";

static const char fs[] = R"___(
uniform sampler2D u_texture;

varying vec2 f_texpos;

void main() {
  gl_FragColor = texture2D(u_texture, f_texpos);
}
)___";

static const GLsizei stride = 4 * sizeof(float);
static const std::vector<gl::AttributeDescriptor> attributes = {
    {"a_position", /**/ 2, GL_FLOAT, stride, gl::offset_float(0)},
    {"a_texpos", /*  */ 2, GL_FLOAT, stride, gl::offset_float(2)},
};
}  // namespace temp

//////////////////////////////////////////////////////////////////////////////

void pixels_of_surface(uint8_t *pixels, cairo_surface_t *surface, int width,
                       int height) {
  cairo_surface_flush(surface);

  uint8_t *data = cairo_image_surface_get_data(surface);
  int stride = cairo_image_surface_get_stride(surface);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int p = width * 3 * y + 3 * x + 0;
      int d = stride * y + 4 * x + 0;

      pixels[p + 0] = data[d + 2];
      pixels[p + 1] = data[d + 1];
      pixels[p + 2] = data[d + 0];
    }
  }
}

//////////////////////////////////////////////////////////////////////////////

static const char *name_of_speed(float speed) {
  if (speed < 0.3) {
    return "Stille";
  }
  if (speed < 1.6) {
    return "Flau vind";
  }
  if (speed < 3.4) {
    return "Svak vind";
  }
  if (speed < 5.5) {
    return "Lett bris";
  }
  if (speed < 8.0) {
    return "Laber bris";
  }
  if (speed < 10.8) {
    return "Frisk bris";
  }
  if (speed < 13.9) {
    return "Liten kuling";
  }
  if (speed < 17.2) {
    return "Stiv kuling";
  }
  if (speed < 20.8) {
    return "Sterk kuling";
  }
  if (speed < 24.5) {
    return "Liten storm";
  }
  if (speed < 28.5) {
    return "Full storm";
  }
  if (speed < 32.7) {
    return "Sterk storm";
  }
  return "Orkan";
}

static const double random_stddev = 0.2;
static const double random_period = 1.5;

struct AppState {
  float wind_speed = 0.0f;

  std::mt19937 random_generator{0};
  std::normal_distribution<> random_distribution{0.0, random_stddev};
  float random_term = 0.0f;
  double random_updated_at = 0.0;

  cairo_font_face_t *cairo_font_face = nullptr;

  void *zmq_ctx = nullptr;
  void *zmq_pub = nullptr;
  void *zmq_sub = nullptr;
};

struct Session {
  platform::Platform platform = {};
  AppState as = {};
};

static Session *the_session;

void reload_main(int argc, char *argv[], void **data, const int *changed) {
  (void)argc;
  (void)argv;

  the_session = static_cast<Session *>(*data);
  if (nullptr == the_session) {
    the_session = new Session;  // NOLINT
    *data = the_session;

    platform::reload_init(&the_session->platform);

    AppState *as = &the_session->as;

    // font
    char *wp_font_path = getenv("WP_FONT_PATH");
    if (nullptr != wp_font_path) {
      FT_Library ft_library;
      {
        FT_Error ft_status = FT_Init_FreeType(&ft_library);
        assert(0 == ft_status);
      }

      FT_Face ft_face;
      {
        FT_Error ft_status = FT_New_Face(ft_library, wp_font_path, 0, &ft_face);
        assert(0 == ft_status);
      }

      as->cairo_font_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);
    } else {
      std::fprintf(stderr, "no custom font (WP_FONT_PATH)\n");
    }

    // ipc
    {
      const char *ipc_pub_url = "ipc:///tmp/wp_pub";
      const char *ipc_sub_url = "ipc:///tmp/wp_sub";

      as->zmq_ctx = zmq_ctx_new();

      as->zmq_pub = zmq_socket(as->zmq_ctx, ZMQ_PUB);
      zmq_connect(as->zmq_pub, ipc_pub_url);

      as->zmq_sub = zmq_socket(as->zmq_ctx, ZMQ_SUB);
      zmq_connect(as->zmq_sub, ipc_sub_url);
      zmq_setsockopt(as->zmq_sub, ZMQ_SUBSCRIBE, "", 0);
    }
  }

  auto &S = *the_session;
  auto p = &S.platform;
  AppState *as = &the_session->as;

  platform::reload_begin(p);

  const int temp_width = 1280;
  const int temp_height = 800;
  const int temp_channels = 3;
  uint8_t temp_image[temp_height * temp_width * temp_channels];

  auto temp_texture = gl::make_texture_2d(GL_LINEAR, GL_LINEAR,
                                          GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

  float temp_vertices[] = {
      -1.0f, -1.0f, +0.0f, +1.0f,  // A
      +1.0f, -1.0f, +1.0f, +1.0f,  // B
      +1.0f, +1.0f, +1.0f, +0.0f,  // C
      -1.0f, -1.0f, +0.0f, +1.0f,  // A
      +1.0f, +1.0f, +1.0f, +0.0f,  // C
      -1.0f, +1.0f, +0.0f, +0.0f,  // D
  };
  auto temp_buffer = gl::make_buffer(temp::vertices);
  auto temp_program = gl::make_program(temp::vs, temp::fs);

  for (;;) {
    platform::frame_begin(p);

    // random wind speed
    {
      double now = platform::time(p);

      if (as->random_updated_at + random_period < now) {
        double r = as->random_distribution(as->random_generator);

        as->random_term = r;
        as->random_updated_at = now;
      }
    }

    for (auto &e : p->events) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
      switch (e.type) {
        case platform::EventType::key_down:
        case platform::EventType::key_repeat: {
          switch (e.key_down.key) {
            case platform::Key::escape:
            case platform::Key::q: {
              platform::exit(p, EXIT_SUCCESS);
            } break;
          }
        } break;
      }
#pragma GCC diagnostic pop
    }
    // double platform_time = platform::time(p);

    for (;;) {
      char buf[1024];

      int numbytes = zmq_recv(as->zmq_sub, buf, sizeof(buf), ZMQ_DONTWAIT);
      if (-1 == numbytes) {
        break;
      }

      {
        const char *topic = "wind_speed ";
        if (0 == strncmp(topic, buf, strlen(topic))) {
          float value;
          if (1 == sscanf(buf + strlen(topic), "%f", &value)) {
            as->wind_speed = value;
          }
        }
      }
    }

    {
      cairo_surface_t *surface = cairo_image_surface_create(
          CAIRO_FORMAT_ARGB32, temp_width, temp_height);

      cairo_t *cr = cairo_create(surface);
      cairo_surface_destroy(surface);

      // background

#if 0
      // dd de e0
      cairo_set_source_rgba(cr, 0.867, 0.871, 0.878, 1.0);
#endif
#if 0
      const double bgl = 0.85;
      cairo_set_source_rgba(cr, bgl * 0.867, bgl * 0.871, bgl * 0.878, 1.0);
#endif
#if 1
      cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
#endif

      cairo_rectangle(cr, 0.0, 0.0, temp_width, temp_height);
      cairo_fill(cr);

      // cairo_set_source_rgba(cr, 0.220, 0.220, 0.220, 1.0);
      // cairo_set_source_rgba(cr, 1.0, 0.0, 0.216, 1.0);
      cairo_set_source_rgba(cr, 0.910, 0.212, 0.263, 1.0);

      // wind speed number
      {
        const double font_size = 270.0;
        const double right = 150.0;
        const double top = 420.0;

        cairo_set_font_face(cr, as->cairo_font_face);
        cairo_set_font_size(cr, font_size);

        float wind_speed = as->wind_speed + as->random_term;
        if (wind_speed < 0.0f) {
          wind_speed = 0.0f;
        }

        char str[100];
        std::sprintf(str, "%3.1f m/s", wind_speed);

        cairo_text_extents_t extents;
        cairo_text_extents(cr, str, &extents);

        double x = 1280.0 - right - extents.width - extents.x_bearing;

        cairo_move_to(cr, x, top);
        cairo_show_text(cr, str);
      }

      // wind speed name
      {
        const double font_size = 100.0;
        const double left = 0.5 * 1280.0;
        const double top = 600.0;

        cairo_set_font_face(cr, as->cairo_font_face);
        cairo_set_font_size(cr, font_size);

        float wind_speed = as->wind_speed + as->random_term;
        if (wind_speed < 0.0f) {
          wind_speed = 0.0f;
        }

        const char *str = name_of_speed(as->wind_speed);

        cairo_text_extents_t extents;
        cairo_text_extents(cr, str, &extents);

        double x = left - 0.5 * extents.width - extents.x_bearing;

        cairo_move_to(cr, x, top);
        cairo_show_text(cr, str);
      }

      pixels_of_surface(temp_image, surface, temp_width, temp_height);

      cairo_destroy(cr);

      glBindTexture(GL_TEXTURE_2D, temp_texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_width, temp_height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, temp_image);
    }

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(temp_program);
    gl::enable(temp_buffer, temp_program, temp::attributes);
    glBufferData(GL_ARRAY_BUFFER, sizeof(temp_vertices), temp_vertices,
                 GL_STATIC_DRAW);
    glBindTexture(GL_TEXTURE_2D, temp_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    ///////////////////////////////////////////////////////////

    if (platform::frame_end(p) || 0 != *changed) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::fprintf(stderr, "\n----------------------------------------\n\n");

  if (platform::reload_end(p)) {
    platform::exit(p, EXIT_SUCCESS);
  }
}
