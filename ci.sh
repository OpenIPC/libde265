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
build "cmake"
build "emcmake cmake -DWASM_SIMD=ON"
build "emcmake cmake -DWASM_SIMD=OFF"
