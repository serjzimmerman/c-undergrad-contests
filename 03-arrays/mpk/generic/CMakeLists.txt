cmake_minimum_required(VERSION 3.0.0)
project(filesort)

add_subdirectory(app)
add_subdirectory(lib)

add_compile_options(
    -std=c90
    --pedantic
    -O0
    -Wall
)