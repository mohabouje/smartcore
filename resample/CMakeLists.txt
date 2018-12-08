cmake_minimum_required(VERSION 3.4)
project(resample++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(${PROJECT_NAME} SHARED include/resample.hpp src/resample.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_link_libraries(${PROJECT_NAME} PRIVATE samplerate)
