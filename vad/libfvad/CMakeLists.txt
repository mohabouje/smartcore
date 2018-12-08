cmake_minimum_required(VERSION 3.4)
project(fvad)

file(GLOB library_src
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"

    "${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/*.c"

    "${CMAKE_CURRENT_SOURCE_DIR}/src/vad/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vad/*.c"
)

add_library(${PROJECT_NAME} SHARED ${library_src})
target_include_directories(${PROJECT_NAME} PUBLIC include/)
