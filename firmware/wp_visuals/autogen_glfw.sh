#!/bin/sh -eu

autoreconf -f -i -v

#./configure PLATFORM=GLFW

./configure PLATFORM=GLFW CXXFLAGS="-O0 -g -stdlib=libc++"  CC=clang-7  CXX=clang++-7 LDFLAGS=-fuse-ld=lld
