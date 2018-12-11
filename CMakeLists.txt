cmake_minimum_required(VERSION 3.4)
project(smartcore)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(thirdparty/librnnoise)
add_subdirectory(thirdparty/libfvad)
add_subdirectory(thirdparty/webrtc)
add_subdirectory(pipeline)

find_package( Eigen3 REQUIRED )
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(/usr/include/webrtc_audio_processing/)

file(GLOB library_src
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${library_src} include/recorder.hpp src/recorder.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/webrtc/)
target_link_libraries(${PROJECT_NAME} PRIVATE
        fvad
        rnnoise
        webrtc-noise
        webrtc-dsp
        webrtc-agc
        portaudio
        speexdsp)
target_compile_options(${PROJECT_NAME} PRIVATE -DWEBRTC_POSIX)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core
        FILES_MATCHING PATTERN "*.hpp*")

