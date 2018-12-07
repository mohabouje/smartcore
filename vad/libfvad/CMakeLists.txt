cmake_minimum_required(VERSION 3.4)
project(fvad)

set(HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fvad.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vad/vad_core.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vad/vad_filterbank.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vad/vad_gmm.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vad/vad_sp.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/signal_processing/resample_by_2_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/signal_processing/signal_processing_library.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/signal_processing/spl_inl.h)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/fvad.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vad/vad_core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vad/vad_filterbank.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vad/vad_gmm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vad/vad_sp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/division_operations.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/energy.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/get_scaling_square.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/resample_48khz.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/resample_by_2_internal.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/resample_fractional.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_processing/spl_inl.c)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include/)
target_include_directories(${PROJECT_NAME} PRIVATE include/vad/)
