cmake_minimum_required(VERSION 3.10)
project(lc)

add_subdirectory(lib)

set(EXEC_SOURCES
  src/main.c
  src/lru.c
)

add_executable(${CMAKE_PROJECT_NAME} ${EXEC_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
add_dependencies(${CMAKE_PROJECT_NAME} hashtable llistc)
target_link_libraries(${CMAKE_PROJECT_NAME} hashtable llistc)