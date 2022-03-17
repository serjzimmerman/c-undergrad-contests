cmake_minimum_required(VERSION 3.7)
project(newton-fractal)

set(CMAKE_VERBOSE_MAKEFILE True)

set(PROJECT_SOURCES
  src/bitmap.c
  src/main.c
  src/shader.c
)

add_definitions(-DVERBOSE)
add_executable(newton-fractal ${PROJECT_SOURCES})

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(newton-fractal glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} m)

install(TARGETS newton-fractal DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})