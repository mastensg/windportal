#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef PLATFORM_GLFW
#include "platform_glfw.cc"
#endif  // PLATFORM_GLFW

#ifdef PLATFORM_VC
#include "platform_vc.cc"
#endif  // PLATFORM_VC
