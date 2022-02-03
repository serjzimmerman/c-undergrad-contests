set (APP_SOURCES
  src/main.c
)

add_executable(test ${APP_SOURCES})
target_link_libraries(test llistc hashtable)