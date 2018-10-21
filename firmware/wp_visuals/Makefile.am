ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -std=c11 -Wall -Wextra -pedantic
AM_CXXFLAGS = -std=c++14 -Wall -Wextra -pedantic

lib_LTLIBRARIES = \
		  libplatform.la \
		  libstbimage.la \
		  libvisuals.la

bin_PROGRAMS = \
	       wp_visuals \
	       wp_visuals_reload

# platform  ############################################################

libplatform_la_CXXFLAGS = $(AM_CXXFLAGS) $(PLATFORM_CFLAGS)
libplatform_la_LIBADD = $(PLATFORM_LIBS)
libplatform_la_SOURCES = \
			 src/platform.h \
			 src/platform.cc

# stbimage  ############################################################

libstbimage_la_CXXFLAGS = $(AM_CXXFLAGS) -Wno-unused-parameter
libstbimage_la_SOURCES = \
			 src/stb/stb_image.h \
			 src/stb_image.cc

# visuals  #############################################################

libvisuals_la_CXXFLAGS = \
			 $(AM_CXXFLAGS) \
			 $(PLATFORM_CFLAGS) \
			 $(CAIRO_CFLAGS) \
			 $(FREETYPE2_CFLAGS) \
			 $(ZMQ_CFLAGS)
libvisuals_la_SOURCES = \
		     src/platform.h \
		     src/visuals.cc \
		     src/reload_main.h

wp_visuals_CXXFLAGS = $(libvisuals_la_CXXFLAGS)
wp_visuals_LDADD = \
	     $(CAIRO_LIBS) \
	     $(FREETYPE2_LIBS) \
	     $(ZMQ_LIBS) \
	     libplatform.la \
	     libvisuals.la
wp_visuals_SOURCES = \
	       src/reload_main.h \
	       src/run.cc

wp_visuals_reload_CXXFLAGS = $(AM_CXXFLAGS) $(PLATFORM_CFLAGS)
wp_visuals_reload_LDADD = \
		    -ldl \
		    -lpthread \
		    $(CAIRO_LIBS) \
		    $(FREETYPE2_LIBS) \
		    $(ZMQ_LIBS) \
		    libplatform.la
wp_visuals_reload_SOURCES = \
		src/reload.cc \
		src/reload_main.h
EXTRA_wp_visuals_reload_DEPENDENCIES = libvisuals.la
