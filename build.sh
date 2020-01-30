#!/bin/bash

usage() {
  echo "usage: $0 <out> <files>"
}

out=$1
[[ -z "${out}" ]] && usage && exit 1
shift 1

[[ "${out}" == "--help" || "${out}" == "-h" ]] && usage && exit 0
[[ $# -lt 1 ]] && usage && exit 1

libs="$(PKG_CONFIG_PATH=/home/joao/code/seastar/seastar-build/lib64/pkgconfig \
  pkg-config --libs --cflags --static seastar)"

g++ -o ${out} $@ ${libs}
