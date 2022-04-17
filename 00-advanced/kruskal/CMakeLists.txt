cmake_minimum_required(VERSION 3.10)
project(kruskal)

option(VERBOSE "Verbose error messages" ON)
if(VERBOSE)
    add_definitions(-DDEBUG)
endif(VERBOSE)

set(EXEC_SOURCES
  src/main.c
  src/udgraph.c
)

add_executable(${CMAKE_PROJECT_NAME} ${EXEC_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)