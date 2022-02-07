set (APP_SOURCES
  src/main.c
  src/util.c
)

add_executable(wordcounter ${APP_SOURCES})
add_dependencies(wordcounter llistc hashtable)
target_link_libraries(wordcounter llistc hashtable)

install(TARGETS wordcounter wordcounter DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)