cmake_minimum_required(VERSION 3.0.0)
project(polynomial)

add_compile_options("$<$<CONFIG:DEBUG>:-pg>")
add_compile_options(-Wall -Wextra)
add_link_options("$<$<CONFIG:DEBUG>:-pg>")

add_subdirectory(app)
add_subdirectory(lib)

set(CMAKE_VERBOSE_MAKEFILE ON)
