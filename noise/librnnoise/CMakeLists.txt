cmake_minimum_required(VERSION 3.4)
project(rnnoise)

file(GLOB library_src
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(${PROJECT_NAME} STATIC ${library_src})
target_include_directories(${PROJECT_NAME} PUBLIC include/)
