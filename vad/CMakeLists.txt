cmake_minimum_required(VERSION 3.4)
project(fvad++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(libfvad)


add_library(${PROJECT_NAME} SHARED include/vad.hpp src/vad.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_include_directories(${PROJECT_NAME} PRIVATE libfvad/include/)
target_link_libraries(${PROJECT_NAME} PRIVATE fvad)
