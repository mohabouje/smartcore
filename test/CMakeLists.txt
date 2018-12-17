cmake_minimum_required(VERSION 3.4)
project(smartcore-test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(GTest REQUIRED)
set(SOURCE_FILES
        testing_gtest.cpp
        gcc_path_test.cpp
        tdoa_test.cpp
        audio_buffer_test.cpp
        acoustic_echo_canceller_test.cpp
        level_test.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
        ${GTEST_LIBRARIES}
        smartcore
        fftw3f
        sndfile
        pthread)
target_compile_options(${PROJECT_NAME} PRIVATE -DUSE_LIBSAMPLERATE -DUSE_LIBSNDFILE -DUSE_LIBFFTW)
target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIRS})
add_test(NAME ${PROJECT_NAME}
        COMMAND ${PROJECT_NAME})