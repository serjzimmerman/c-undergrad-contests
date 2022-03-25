cmake_minimum_required(VERSION 3.7)
project(newton-fractal)

set(CMAKE_VERBOSE_MAKEFILE True)

set(PROJECT_SOURCES
  src/bitmap.c
  src/main.c
  src/shader.c
)

add_subdirectory(lib)

add_definitions(-DVERBOSE)
add_executable(newton-fractal ${PROJECT_SOURCES})
add_dependencies(newton-fractal hashtable)

# glfw
find_package(glfw3 REQUIRED)
# opengl
find_package(OpenGL REQUIRED)
# glew
find_package(GLEW REQUIRED)

target_link_libraries(newton-fractal glfw ${OPENGL_LIBRARIES} ${GLFW_LIBRARY_DIRS} ${GLEW_LIBRARIES} cglm_headers m hashtable)
target_include_directories(newton-fractal PRIVATE ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
install(TARGETS newton-fractal DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})