cmake_minimum_required(VERSION 3.10)
project(lc)

set(EXEC_SOURCES
  src/main.c
  src/lruhash.c
  src/lru.c
)

add_executable(${CMAKE_PROJECT_NAME} ${EXEC_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)