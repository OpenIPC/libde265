set (wasm_sources
  simd.cc simd.h
)

set (wasm_simd_sources
  simd-motion.cc simd-motion.h simd-dct.h simd-dct.cc
)

add_library(wasm OBJECT ${wasm_sources})

add_library(wasm_simd OBJECT ${wasm_simd_sources})

set(wasm_flags "-msse4.2 -msimd128 -fno-vectorize")

set(WASM_OBJECTS $<TARGET_OBJECTS:wasm> $<TARGET_OBJECTS:wasm_simd> PARENT_SCOPE)

set_target_properties(wasm_simd PROPERTIES COMPILE_FLAGS "${wasm_flags}")
