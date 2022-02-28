set(LIB_SOURCES
    src/hashtable.c
    src/counter.c
    src/spair.c
)

add_library(hashtable_n STATIC ${LIB_SOURCES})
add_dependencies(hashtable_n llistc)
target_compile_definitions(hashtable_n PRIVATE -DUSE_CHAIN_POINTER_N_OPTIMIZATION)

add_library(hashtable STATIC ${LIB_SOURCES})
add_dependencies(hashtable llistc)

target_link_libraries(hashtable_n llistc)
target_include_directories(hashtable_n PUBLIC include)
target_link_libraries(hashtable llistc)
target_include_directories(hashtable PUBLIC include)

add_subdirectory(test)