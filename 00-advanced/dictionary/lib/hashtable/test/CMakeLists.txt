set (APP_SOURCES
  src/main.c
  src/util.c
)

add_executable(test ${APP_SOURCES})
target_link_libraries(test llistc hashtable)

install(TARGETS test DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)