cmake_minimum_required(VERSION 3.7)
project(newton-fractal)

set(PROJECT_SOURCES
  src/sdlutil.cc
  src/julia.cc
)

add_executable(newton-fractal ${PROJECT_SOURCES})

find_package(SDL2 REQUIRED)
include_directories(newton-fractal ${SDL2_INCLUDE_DIRS})
target_link_libraries(newton-fractal ${SDL2_LIBRARIES})