#!/bin/sh -eu

base="${1}"
shift

program="${base}_reload"

make "${program}" && ./libtool --mode=execute cgdb "${program}"
