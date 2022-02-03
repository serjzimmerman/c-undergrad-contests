cmake_minimum_required(VERSION 3.0.0)
project(dictionary)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")

add_compile_options("$<$<CONFIG:DEBUG>:-pg>")
add_link_options("$<$<CONFIG:DEBUG>:-pg>")

add_subdirectory(lib)