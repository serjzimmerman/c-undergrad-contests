cmake_minimum_required(VERSION 3.17)
project(topdown-parsing)

set(PROJECT_SOURCES
  src/main.c
  src/lexer.c
  src/parser.c
  src/token.c
)

add_definitions(-DDEBUG)

add_executable(topdown-parsing ${PROJECT_SOURCES})