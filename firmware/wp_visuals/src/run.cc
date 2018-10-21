#ifdef HAVE_CONFIG_H
#include "config.h"  // IWYU pragma: keep
#endif

#include "reload_main.h"

int main(int argc, char *argv[]) {
  void *data = nullptr;
  int changed = 0;

  reload_main(argc, argv, &data, &changed);
}
