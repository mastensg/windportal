#!/bin/sh -eu

base="${1}"
shift

program="${base}_reload"
library=".libs/lib${base}.so"

set -x

make "${program}" && "./${program}" "${library}" "${@}"
