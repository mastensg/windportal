#!/bin/sh -eu

autoreconf -f -i -v

./configure PLATFORM=VC VCDIR=./vc --build=x86_64-linux-gnu --host=arm-linux-gnueabihf
