cmake_minimum_required(VERSION 3.0.0)
project(dictionary)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options("$<$<CONFIG:DEBUG>:-pg>")
add_compile_options(-Wall -Wextra)
add_link_options("$<$<CONFIG:DEBUG>:-pg>")

add_subdirectory(lib)
add_subdirectory(app)