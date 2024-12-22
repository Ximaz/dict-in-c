cmake_minimum_required(VERSION 3.31.1)

project(
  Dict
  VERSION 1.0.0
  DESCRIPTION "Dict datastructure implementation in C."
  LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_library(dict STATIC)

option(CODE_COVERAGE "Enable code coverage reporting" OFF)

if(CODE_COVERAGE)
  message(STATUS "Code coverage enabled")
  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(dict PUBLIC --coverage -fprofile-arcs -ftest-coverage)
    target_link_options(dict PUBLIC --coverage -fprofile-arcs -ftest-coverage)
  else()
    message(WARNING "Code coverage is not supported for this compiler")
  endif()
endif()

set(MY_CFLAGS
  "-Wall"
  "-Wextra"
  "-Werror"
  "-pedantic"
  "-std=c99"
  "-fPIC"
  "-fno-delete-null-pointer-checks"
  "-fno-strict-overflow"
  "-fno-strict-aliasing"
  "-ftrivial-auto-var-init=zero"
  "-Wformat"
  "-Wimplicit-fallthrough"
  "-U_FORTIFY_SOURCE"
  "-D_GLIBCXX_ASSERTIONS"
  "-fstack-protector-strong"
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  list(APPEND MY_CFLAGS
    "-fcf-protection=full"
    "-Wl,-z,nodlopen"
    "-Wl,-z,noexecstack"
    "-Wl,-z,relro"
    "-Wl,-z,now"
    "-fstack-clash-protection"
    "-fstrict-flex-arrays=3"
    "-Wtrampolines"
  )
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
  list(APPEND MY_CFLAGS "-mbranch-protection=standard")
endif()

target_include_directories(
  dict PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_sources(
  dict PRIVATE
  "src/murmurhash1.c"
  "src/dict_ctor.c"
  "src/dict_dtor.c"
  "src/entry_ctor.c"
  "src/entry_dtor.c"
  "src/dict_buckets_ctor.c"
  "src/dict_buckets_dtor.c"
  "src/dict_bucket_insert.c"
  "src/dict_insert.c"
  "src/dict_resize.c"
)
target_compile_options(dict PRIVATE ${MY_CFLAGS})

enable_testing()
add_subdirectory(tests)
