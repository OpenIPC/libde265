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
add_library(x86_avx OBJECT ${x86_avx_sources})

set(sse_flags "")
set(avx_flags "")

if(NOT MSVC)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(sse_flags "${sse_flags} -msse4.1")
  else(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(sse_flags "${sse_flags} -msse2 -mssse3 -msse4.1")
  endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif()

aif(MSVC)
  set(avx_flags "${avx_flags} /arch:AVX")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set(avx_flags "${avx_flags} /QxAVX")
else()
  set(avx_flags "${avx_flags} -mavx")
endif()

set(X86_OBJECTS $<TARGET_OBJECTS:x86> $<TARGET_OBJECTS:x86_sse> $<TARGET_OBJECTS:x86_avx> PARENT_SCOPE)

SET_TARGET_PROPERTIES(x86_sse PROPERTIES COMPILE_FLAGS "${sse_flags}")
SET_TARGET_PROPERTIES(x86_avx PROPERTIES COMPILE_FLAGS "${avx_flags}")
