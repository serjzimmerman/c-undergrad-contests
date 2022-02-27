set (APP_SOURCES
  src/main.c
  src/util.c
)

add_executable(testmemoryoptimized ${APP_SOURCES})
target_link_libraries(testmemoryoptimized llistc hashtable)

add_executable(testspeedoptimized ${APP_SOURCES})
target_link_libraries(testspeedoptimized llistc hashtable)
target_compile_definitions(testspeedoptimized PUBLIC -DUSE_CHAIN_POINTER_N_OPTIMIZATION)

install(TARGETS testmemoryoptimized testspeedoptimized DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)