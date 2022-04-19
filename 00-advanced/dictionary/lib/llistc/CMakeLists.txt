set(LIB_SOURCES
    src/sllistc.c
)

add_library(llistc STATIC ${LIB_SOURCES})
target_include_directories(llistc PUBLIC include)