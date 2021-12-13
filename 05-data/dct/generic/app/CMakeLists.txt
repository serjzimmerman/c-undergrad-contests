set (APP_SOURCES
  src/counter.c
  src/main.c
)

add_executable(counter ${APP_SOURCES})
target_link_libraries(counter hashtable)