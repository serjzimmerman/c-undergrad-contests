cmake_minimum_required(VERSION 3.0.0)
project(dictionary)

set(CMAKE_VERBOSE_MAKEFILE ON)

option(VERBOSE "VERBOSE" OFF)

add_compile_options("$<$<CONFIG:DEBUG>:-pg>")
add_compile_options(-Wall -Wextra)
add_link_options("$<$<CONFIG:DEBUG>:-pg>")

if(VERBOSE)
  add_compile_definitions(VERBOSE)
endif()

add_subdirectory(lib)
add_subdirectory(app)