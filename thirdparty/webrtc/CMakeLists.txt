cmake_minimum_required(VERSION 3.4)
project(webrtc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(DSP_FILES
        common_audio/signal_processing/include/signal_processing_library.h
        common_audio/signal_processing/include/spl_inl.h
        common_audio/signal_processing/auto_corr_to_refl_coef.c
        common_audio/signal_processing/auto_corr_to_refl_coef.c
        common_audio/signal_processing/complex_bit_reverse.c
        common_audio/signal_processing/copy_set_operations.c
        common_audio/signal_processing/complex_fft.c
        common_audio/signal_processing/cross_correlation.c
        common_audio/signal_processing/division_operations.c
        common_audio/signal_processing/downsample_fast.c
        common_audio/signal_processing/energy.c
        common_audio/signal_processing/filter_ar.c
        common_audio/signal_processing/get_hanning_window.c
        common_audio/signal_processing/get_scaling_square.c
        common_audio/signal_processing/ilbc_specific_functions.c
        common_audio/signal_processing/levinson_durbin.c
        common_audio/signal_processing/lpc_to_refl_coef.c
        common_audio/signal_processing/min_max_operations.c
        common_audio/signal_processing/randomization_functions.c
        common_audio/signal_processing/real_fft.c
        common_audio/signal_processing/refl_coef_to_lpc.c
        common_audio/signal_processing/resample_fractional.c
        common_audio/signal_processing/resample.c
        common_audio/signal_processing/resample_48khz.c
        common_audio/signal_processing/resample_by_2.c
        common_audio/signal_processing/resample_by_2_internal.h
        common_audio/signal_processing/resample_by_2_internal.c
        common_audio/signal_processing/spl_init.c
        common_audio/signal_processing/spl_sqrt.c
        common_audio/signal_processing/splitting_filter.c
        common_audio/signal_processing/sqrt_of_one_minus_x_squared.c
        common_audio/signal_processing/vector_scaling_operations.c
        common_audio/signal_processing/dot_product_with_scale.h
        common_audio/signal_processing/dot_product_with_scale.cc
        common_audio/third_party/fft4g/fft4g.h
        common_audio/third_party/fft4g/fft4g.c
        common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.c
        common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.h
        common_audio/include/audio_util.h
        common_audio/audio_util.cc)

add_library(webrtc-dsp SHARED ${DSP_FILES})
target_include_directories(webrtc-dsp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(webrtc-dsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_processing/)
target_include_directories(webrtc-dsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common_audio/include/)
target_compile_definitions(webrtc-dsp PRIVATE -DWEBRTC_POSIX)
set_target_properties(webrtc-dsp PROPERTIES PUBLIC_HEADER
        common_audio/signal_processing/include/signal_processing_library.h)

include(GNUInstallDirs)
install(TARGETS webrtc-dsp
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


set(NS_FILES
        modules/audio_processing/ns/defines.h
        modules/audio_processing/ns/noise_suppression.h
        modules/audio_processing/ns/noise_suppression.c
        modules/audio_processing/ns/noise_suppression_x.h
        modules/audio_processing/ns/noise_suppression_x.c
        modules/audio_processing/ns/ns_core.h
        modules/audio_processing/ns/ns_core.c
        modules/audio_processing/ns/nsx_core_c.c
        modules/audio_processing/ns/nsx_core.h
        modules/audio_processing/ns/nsx_core.c
        modules/audio_processing/ns/nsx_defines.h
        modules/audio_processing/ns/windows_private.h
        rtc_base/checks.h
        rtc_base/checks.cc)

add_library(webrtc-noise SHARED ${NS_FILES})
target_include_directories(webrtc-noise PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(webrtc-noise PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_processing/ns/)
target_compile_definitions(webrtc-noise PRIVATE -D__native_client__)
target_link_libraries(webrtc-noise PRIVATE webrtc-dsp)
set_target_properties(webrtc-noise PROPERTIES PUBLIC_HEADER modules/audio_processing/ns/noise_suppression.h)

include(GNUInstallDirs)
install(TARGETS webrtc-noise
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


set(AGC_FILES
        modules/audio_processing/agc/legacy/gain_control.h
        modules/audio_processing/agc/legacy/digital_agc.h
        modules/audio_processing/agc/legacy/digital_agc.c
        modules/audio_processing/agc/legacy/analog_agc.h
        modules/audio_processing/agc/legacy/analog_agc.c)

add_library(webrtc-agc SHARED ${AGC_FILES})
target_include_directories(webrtc-agc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(webrtc-agc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_processing/agc/legacy/)
target_compile_definitions(webrtc-agc PRIVATE -D__native_client__)
target_link_libraries(webrtc-agc PRIVATE webrtc-dsp)
set_target_properties(webrtc-agc PROPERTIES PUBLIC_HEADER modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-agc
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")
