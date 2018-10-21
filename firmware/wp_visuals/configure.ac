AC_INIT([wp_visuals], [0])
AC_PREREQ([2.68])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([src/platform.cc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_SILENT_RULES([yes])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AM_PROG_AR
LT_INIT([disable-static])

PKG_CHECK_MODULES([CAIRO], [cairo])
PKG_CHECK_MODULES([FREETYPE2], [freetype2])
PKG_CHECK_MODULES([ZMQ], [libzmq])

# PLATFORM  ############################

AC_ARG_VAR([PLATFORM], [GLFW or VC.])

AS_IF([test "x$PLATFORM" = "x"], [PLATFORM="GLFW"])

AM_CONDITIONAL(PLATFORM_GLFW, [test "x$PLATFORM" = "xGLFW"])
AM_CONDITIONAL(PLATFORM_VC, [test "x$PLATFORM" = "xVC"])

AS_IF(
      [test "x$PLATFORM" = "xGLFW"], [
       AC_DEFINE([PLATFORM_GLFW], [1], [Platform is GLFW])
       AC_DEFINE([OPENGL_PROFILE_CORE], [1], [OpenGL profile is Core])
       PKG_CHECK_MODULES([GL], [gl])
       PKG_CHECK_MODULES([GLEW], [glew])
       PKG_CHECK_MODULES([GLFW], [glfw3])
       OPENGL_CFLAGS="${GL_CFLAGS} ${GLEW_CFLAGS}"
       OPENGL_LIBS="${GL_LIBS} ${GLEW_LIBS}"
       PLATFORM_CFLAGS="${OPENGL_CFLAGS} ${GLFW_CFLAGS}"
       PLATFORM_LIBS="${OPENGL_LIBS} ${GLFW_LIBS}"
       ],
      [test "x$PLATFORM" = "xVC"], [
       AC_DEFINE([PLATFORM_VC], [1], [Platform is VC])
       AC_DEFINE([OPENGL_PROFILE_ES], [1], [OpenGL profile is ES])
       PKG_CHECK_MODULES([GLES], [brcmglesv2])
       PKG_CHECK_MODULES([BCM_HOST], [bcm_host])
       PKG_CHECK_MODULES([EGL], [brcmegl])
       PKG_CHECK_MODULES([INPUT], [libinput])
       OPENGL_CFLAGS="${GLES_CFLAGS}"
       OPENGL_LIBS="${GLES_LIBS}"
       PLATFORM_CFLAGS="${OPENGL_CFLAGS} ${BCM_HOST_CFLAGS} ${EGL_CFLAGS} ${INPUT_CFLAGS}"
       PLATFORM_LIBS="${OPENGL_LIBS} ${BCM_HOST_LIBS} ${EGL_LIBS} ${INPUT_LIBS}"
       ])

AC_SUBST([PLATFORM_CFLAGS])
AC_SUBST([PLATFORM_LIBS])

########################################

AC_ARG_VAR([TIDY], [clang-tidy command])
AM_CONDITIONAL(ENABLE_TIDY, [test "x$TIDY" != "x"])

########################################

AC_OUTPUT
