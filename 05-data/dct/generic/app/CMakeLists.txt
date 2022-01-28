set (APP_SOURCES
  src/main.c
  src/hashtable.c
  src/counter.c
)

add_executable(counter ${APP_SOURCES})
target_link_libraries(counter llistc)