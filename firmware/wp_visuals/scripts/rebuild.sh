#!/bin/sh -eu

base="${1}"

lt_library="lib${base}.la"

git ls-files | entr -c make "${lt_library}"
