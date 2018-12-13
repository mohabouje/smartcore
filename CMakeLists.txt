cmake_minimum_required(VERSION 3.4)
project(smartcore)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(thirdparty)
add_subdirectory(pipeline)
add_subdirectory(test)


file(GLOB library_src
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

find_package(SndFile REQUIRED)
find_package(rnnoise REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(FFTW REQUIRED)
find_package(Samplerate REQUIRED)
find_package(Portaudio REQUIRED)
find_package(SpeexDSP REQUIRED)


add_library(${PROJECT_NAME} SHARED ${library_src})
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/webrtc/)
target_compile_options(${PROJECT_NAME} PRIVATE -DUSE_LIBSAMPLERATE -DUSE_LIBSNDFILE -DUSE_LIBFFTW)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${SNDFILE_INCLUDE_DIRS}
        ${RNNOISE_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIR}
        ${FFTW_INCLUDE_DIRS}
        ${SAMPLERATE_INCLUDE_DIRS}
        ${PORTAUDIO_INCLUDE_DIRS}
        ${SPEEXDSP_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
        webrtc-noise
        webrtc-dsp
        webrtc-agc
        webrtc-aec
        webrtc-red
        webrtc-rnn-vad
        fvad
        ${FFTW_LIBRARIES}
        ${SNDFILE_LIBRARIES}
        ${RNNOISE_LIBRARIES}
        ${SAMPLERATE_LIBRARIES}
        ${PORTAUDIO_LIBRARIES}
        ${SPEEXDSP_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE -DWEBRTC_POSIX)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core
        FILES_MATCHING PATTERN "*.hpp*")

