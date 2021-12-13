set(LIB_SOURCES
    src/hashtable.c
)

set(TEST_SOURCES
    test/src/test.c
)

add_library(hashtable STATIC ${LIB_SOURCES})
target_include_directories(hashtable PUBLIC include)

add_compile_options(
    --pedantic
    -Wall
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-D_DEBUG")
    add_executable(hashtable_test ${TEST_SOURCES})
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-D_RELEASE")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")