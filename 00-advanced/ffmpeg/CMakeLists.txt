cmake_minimum_required(VERSION 3.17)
project(ffmpeg-test)

set(PROJECT_SOURCES
  src/main.c
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

add_executable(ffmpeg-test ${PROJECT_SOURCES})
target_link_libraries(ffmpeg-test PkgConfig::LIBAV)