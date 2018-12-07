cmake_minimum_required(VERSION 3.4)
project(fvad++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(librnnoise)


add_library(${PROJECT_NAME} STATIC include/noise_suppression.hpp src/noise_suppression.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include/)
target_include_directories(${PROJECT_NAME} PRIVATE librnnoise/include/)
target_link_libraries(${PROJECT_NAME} PRIVATE rnnoise)
