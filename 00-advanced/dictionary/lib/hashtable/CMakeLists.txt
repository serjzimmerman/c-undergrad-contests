set(LIB_SOURCES
    src/hashtable.c
    src/counter.c
)

# add_compile_definitions(USE_CHAIN_POINTER_N_OPTIMIZATION)

add_library(hashtable STATIC ${LIB_SOURCES})
add_dependencies(hashtable llistc)

target_link_libraries(hashtable llistc)
target_include_directories(hashtable PUBLIC include)

add_subdirectory(test)