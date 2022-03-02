set (APP_SOURCES
  src/main.c
  src/util.c
)

add_executable(fstrings ${APP_SOURCES})
add_dependencies(fstrings llistc hashtable)
target_link_libraries(fstrings llistc hashtable)

install(TARGETS fstrings fstrings DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)