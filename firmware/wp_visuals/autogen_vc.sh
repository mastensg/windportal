#!/bin/sh -eu

autoreconf -f -i -v

CPPFLAGS="-isystem /opt/vc/include \
    -isystem /opt/vc/include/interface/vcos/pthreads \
    -isystem /opt/vc/include/interface/vmcs_host/linux/"

./configure \
    PLATFORM=VC \
    PKG_CONFIG_PATH="/opt/vc/lib/pkgconfig" \
    CPPFLAGS="${CPPFLAGS}" \
    CFLAGS="-O0 -g" \
    CXXFLAGS="-O0 -g"
