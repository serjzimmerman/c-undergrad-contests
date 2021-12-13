cmake_minimum_required(VERSION 3.0.0)
project(counter)

add_subdirectory(app)
add_subdirectory(lib)

install(TARGETS counter DESTINATION ${CMAKE_BINARY_DIR}/..)