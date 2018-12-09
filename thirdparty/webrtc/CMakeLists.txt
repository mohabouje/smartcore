cmake_minimum_required(VERSION 3.4)
project(webrtc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(NS_FILES
        modules/audio_processing/ns/defines.h
        modules/audio_processing/ns/noise_suppression.h
        modules/audio_processing/ns/noise_suppression.c
        modules/audio_processing/ns/ns_core.h
        modules/audio_processing/ns/ns_core.c
        modules/audio_processing/ns/windows_private.h)

add_library(webrtc-noise SHARED ${NS_FILES})
target_include_directories(webrtc-noise PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(webrtc-noise PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_processing/ns/)
set_target_properties(webrtc-noise PROPERTIES PUBLIC_HEADER modules/audio_processing/ns/noise_suppression.h)

include(GNUInstallDirs)
install(TARGETS webrtc-noise
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")