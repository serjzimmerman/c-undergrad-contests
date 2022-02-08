set(EXECUTABLE_SOURCES
    src/main.c
)

add_executable(generic ${EXECUTABLE_SOURCES})

target_link_libraries(generic polynomial)

install(TARGETS generic DESTINATION ${PROJECT_SOURCE_DIR})