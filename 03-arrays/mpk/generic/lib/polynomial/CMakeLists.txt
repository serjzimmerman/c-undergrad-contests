set(LIB_SOURCES
    src/polynomial.c
)

add_library(polynomial STATIC ${LIB_SOURCES})
target_include_directories(polynomial INTERFACE include)