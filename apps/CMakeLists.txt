cmake_minimum_required(VERSION 3.4)
project(smartcore-pipeline)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE smartcore)


find_package(Boost COMPONENTS program_options REQUIRED)
add_executable(smartcore-recorder recorder.cpp)
target_link_libraries(smartcore-recorder PRIVATE smartcore ${Boost_LIBRARIES})
