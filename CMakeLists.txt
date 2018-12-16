cmake_minimum_required(VERSION 3.4)
project(smartcore)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

option(USE_RNNOISE "If enabled, it will add the RNNoise library to the build system" OFF)
option(COMPILE_APPS "If enabled, it will compile the different applications" ON)
option(COMPILE_TEST "If enabled, it will compile the different tests" ON)

add_subdirectory(thirdparty)

find_package(SndFile REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(FFTW REQUIRED)
find_package(Samplerate REQUIRED)
find_package(Portaudio REQUIRED)
find_package(SpeexDSP REQUIRED)
file(GLOB sources
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(libraries
        aubio
        maximilian
        webrtc-noise
        webrtc-dsp
        webrtc-agc
        webrtc-aec
        webrtc-red
        webrtc-rnn-vad
        fvad
        usb-1.0
        ${FFTW_FLOAT_LIB}
        ${SNDFILE_LIBRARIES}
        ${SAMPLERATE_LIBRARIES}
        ${PORTAUDIO_LIBRARIES}
        ${SPEEXDSP_LIBRARIES})

set(include_dirs
        ${SNDFILE_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIR}
        ${FFTW_INCLUDE_DIRS}
        ${SAMPLERATE_INCLUDE_DIRS}
        ${PORTAUDIO_INCLUDE_DIRS}
        ${SPEEXDSP_INCLUDE_DIRS})

if (USE_RNNOISE)
    find_package(rnnoise REQUIRED)
    list(APPEND libraries ${RNNOISE_LIBRARIES})
    list(APPEND includ_dirs ${RNNOISE_INCLUDE_DIRS})
else()
    list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/include/rnn_noise_suppression.hpp)
    list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/src/rnn_noise_suppression.cpp)
endif()

if (COMPILE_APPS)
    add_subdirectory(apps)
endif()

if (COMPILE_TEST)
    add_subdirectory(test)
endif()

add_library(${PROJECT_NAME} SHARED ${sources})
target_link_libraries(${PROJECT_NAME} PRIVATE ${libraries})
target_include_directories(${PROJECT_NAME} PRIVATE ${include_dirs})
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/webrtc/ thirdparty/beamforming)
target_compile_options(${PROJECT_NAME} PRIVATE -DUSE_LIBSAMPLERATE -DUSE_LIBSNDFILE -DUSE_LIBFFTW)
target_compile_options(${PROJECT_NAME} PRIVATE -DWEBRTC_POSIX)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core
        FILES_MATCHING PATTERN "*.hpp*")

