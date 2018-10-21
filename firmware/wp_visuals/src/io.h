#ifndef IO_H
#define IO_H

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>

#include <err.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "sl.h"

namespace io {

inline void *map_file(const char *path, off_t *numbytes) {
  int fd;

  if (-1 == (fd = open(path, O_RDONLY | O_CLOEXEC))) {  // NOLINT
    err(EXIT_FAILURE, "open(\"%s\")", path);
  }

  if (-1 == (*numbytes = lseek(fd, 0, SEEK_END))) {
    err(EXIT_FAILURE, "lseek");
  }

  void *p = mmap(nullptr, *numbytes, PROT_READ, MAP_SHARED, fd, 0);
  if (MAP_FAILED == p) {  // NOLINT
    err(EXIT_FAILURE, "mmap");
  }

  close(fd);

  return p;
}

inline bool file_is_readable(const char *path) {
  std::ifstream is(path, std::ios::in);
  return is.good();
}

struct string_of_file_ret {
  bool ok;
  std::string contents;
};

inline string_of_file_ret string_of_file(const char *path) {
  std::ifstream is(path, std::ios::in | std::ios::binary | std::ios::ate);
  if (is.fail()) {
    return {false, ""};
  }

  string_of_file_ret r;

  size_t size = is.tellg();
  r.contents.resize(size);
  is.seekg(0);

  is.read(&r.contents[0], size);

  r.ok = is.good();

  return r;
}

inline void save_ppm(const char *path, uint8_t *pixels, int width, int height,
                     int channels) {
  FILE *f = fopen(path, "w");
  assert(nullptr != f);

  std::fprintf(f, "P3\n");
  std::fprintf(f, "%d %d\n", width, height);
  std::fprintf(f, "%d\n", 255);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      for (int c = 0; c < channels; ++c) {
        int i = y * width * channels + x * channels + c;
        int p = pixels[i];

        std::fprintf(f, "%3d ", p);
      }
    }
  }

  fclose(f);
}

inline void print_mat4(sl::mat4 m) {
  // clang-format off
  std::fprintf(stderr, "%6.03f %6.03f %6.03f %6.03f\n", m.e00, m.e01, m.e02, m.e03);
  std::fprintf(stderr, "%6.03f %6.03f %6.03f %6.03f\n", m.e10, m.e11, m.e12, m.e13);
  std::fprintf(stderr, "%6.03f %6.03f %6.03f %6.03f\n", m.e20, m.e21, m.e22, m.e23);
  std::fprintf(stderr, "%6.03f %6.03f %6.03f %6.03f\n", m.e30, m.e31, m.e32, m.e33);
  std::fprintf(stderr, "\n");
  // clang-format on
}

}  // namespace io

#endif  // IO_H
