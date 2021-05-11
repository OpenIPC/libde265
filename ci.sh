#!/usr/bin/env bash


function build {
  rm -rf build
  $1 -H. -Bbuild $2|| return 1
  pushd build
  make -j$(nproc) || return 1
  popd
  return 0
}

set -e
build "cmake -DDISABLE_SIMD=ON"
build "cmake -DDISABLE_SIMD=OFF"
build "emcmake cmake -DDISABLE_SIMD=ON"
build "emcmake cmake -DDISABLE_SIMD=OFF"
