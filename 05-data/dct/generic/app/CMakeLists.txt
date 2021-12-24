set (APP_SOURCES
  src/counter.c
  src/main.c
  src/hashtable.c
)

add_executable(counter ${APP_SOURCES})
target_link_libraries(counter)