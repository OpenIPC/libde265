set (x86_sources
  sse.cc sse.h
)

set (x86_sse_sources
  sse-motion.cc sse-motion.h sse-dct.h sse-dct.cc
)

set (x86_avx_sources
  avx-motion.cc
)

add_library(x86 OBJECT ${x86_sources})

add_library(x86_sse OBJECT ${x86_sse_sources})
set(sse_flags "")

if(MSVC)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set(sse_flags "${sse_flags} -xSSE4.2")
else()
  set(sse_flags "${sse_flags} -msse4.1")
endif()

if (SUPPORTS_AVX)
  add_library(x86_avx OBJECT ${x86_avx_sources})
  set(avx_flags "")

  if(MSVC)
    set(avx_flags "${avx_flags} /arch:AVX")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(avx_flags "${avx_flags} -xAVX")
  else()
    set(avx_flags "${avx_flags} -mavx2")
  endif()
endif()

set(X86_OBJECTS $<TARGET_OBJECTS:x86> $<TARGET_OBJECTS:x86_sse> $<TARGET_OBJECTS:x86_avx> PARENT_SCOPE)

SET_TARGET_PROPERTIES(x86_sse PROPERTIES COMPILE_FLAGS "${sse_flags}")
SET_TARGET_PROPERTIES(x86_avx PROPERTIES COMPILE_FLAGS "${avx_flags}")
