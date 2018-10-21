#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <array>
#include <chrono>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

#include <dlfcn.h>
#include <err.h>
#include <libgen.h>
#include <linux/limits.h>
#include <sys/inotify.h>
#include <unistd.h>

#include "reload_main.h"

using reload_main_t = decltype(&reload_main);

static void watch_for_change(const char *path, int *changed) {
  static char path_for_basename[PATH_MAX];
  static char path_for_dirname[PATH_MAX];

  strncpy(path_for_basename, path, PATH_MAX);
  const char *file = basename(path_for_basename);

  strncpy(path_for_dirname, path, PATH_MAX);
  const char *directory = dirname(path_for_dirname);

  int fd = inotify_init1(IN_CLOEXEC);
  if (fd == -1) {
    err(EXIT_FAILURE, "inotify_init");
  }

  int wd = inotify_add_watch(fd, directory, IN_ATTRIB);
  if (-1 == wd) {
    err(EXIT_FAILURE, "inotify_add_watch");
  }

  for (;;) {
    alignas(std::alignment_of<struct inotify_event>::value)
        std::array<uint8_t, sizeof(struct inotify_event) + NAME_MAX + 1>
            buf{};

    if (-1 == read(fd, buf.data(), sizeof(buf))) {
      err(EXIT_FAILURE, "read");
    }

    auto *ie = reinterpret_cast<struct inotify_event *>(buf.data());

    if (0 != std::strcmp(file, static_cast<char *>(ie->name))) {
      continue;
    }

    *changed = 1;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::fprintf(stderr, "usage: %s ./library.so [...]\n", argv[0]);
    return EXIT_FAILURE;
  }

  const int lib_argc = argc - 1;
  char **lib_argv = argv + 1;

  static char real_path[PATH_MAX];
  {
    const char *path = argv[1];
    static char path_for_realpath[PATH_MAX];

    strncpy(path_for_realpath, path, PATH_MAX);
    const char *realpath_ret = realpath(path_for_realpath, real_path);
    if (realpath_ret != real_path) {
      err(EXIT_FAILURE, "realpath");
    }
  }

  int changed = 0;
  std::thread watch_thread(watch_for_change, real_path, &changed);

  void *data = nullptr;

  for (;;) {
    auto entered_at = std::chrono::high_resolution_clock::now();

    void *lib = dlopen(real_path, RTLD_LAZY);
    if (nullptr == lib) {
      std::fprintf(stderr, "dlopen: %s\n", dlerror());
    } else {
      auto lib_reload_main =
          reinterpret_cast<reload_main_t>(dlsym(lib, "reload_main"));
      if (nullptr == lib_reload_main) {
        std::fprintf(stderr, "dlsym: %s\n", dlerror());
      } else {
        lib_reload_main(lib_argc, lib_argv, &data, &changed);
      }
      if (0 != dlclose(lib)) {
        std::fprintf(stderr, "dlclose: %s\n", dlerror());
      }
    }

    changed = 0;
    std::this_thread::sleep_until(entered_at + std::chrono::seconds(1));
  }
}
