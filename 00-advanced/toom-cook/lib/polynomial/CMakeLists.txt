set(LIB_SOURCES
    src/mat.c
    src/frac.c
    src/poly.c
)

add_library(polynomial STATIC ${LIB_SOURCES})

target_include_directories(polynomial PUBLIC include)