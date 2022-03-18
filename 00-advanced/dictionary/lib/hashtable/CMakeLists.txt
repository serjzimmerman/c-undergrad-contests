set(LIB_SOURCES
    src/hashtable.c
    src/counter.c
    src/spair.c
)

option(USE_N_OPTIMIZATION "USE_N_OPTIMIZATION" OFF)

add_library(hashtable STATIC ${LIB_SOURCES})
add_dependencies(hashtable llistc)
target_link_libraries(hashtable llistc)
target_include_directories(hashtable PUBLIC include)

if(USE_N_OPTIMIZATION)
  add_compile_definitions(hashtabl PRIVATE USE_CHAIN_POINTER_N_OPTIMIZATION)
endif()

add_subdirectory(test)