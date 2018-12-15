cmake_minimum_required(VERSION 3.4)
project(webrtc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(maximilian SHARED maximilian/maximilian.h maximilian/maximilian.cpp)
target_include_directories(maximilian PUBLIC maximilian)
install(TARGETS maximilian
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


set(SYSTEM_FILES
        webrtc/system_wrappers/include/metrics.h
        webrtc/system_wrappers/source/metrics.cc
        webrtc/system_wrappers/include/cpu_features_wrapper.h
        webrtc/system_wrappers/source/cpu_features.cc
        webrtc/rtc_base/memory/aligned_malloc.h
        webrtc/rtc_base/memory/aligned_malloc.cc
        webrtc/rtc_base/criticalsection.h
        webrtc/rtc_base/criticalsection.cc
        webrtc/rtc_base/platform_thread_types.h
        webrtc/rtc_base/platform_thread_types.cc
        webrtc/rtc_base/checks.h
        webrtc/rtc_base/checks.cc)

add_library(webrtc-system SHARED ${SYSTEM_FILES})
target_include_directories(webrtc-system PRIVATE webrtc)
target_compile_definitions(webrtc-system PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
install(TARGETS webrtc-system
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")

set(UTIL_FILES
        webrtc/modules/audio_processing/utility/block_mean_calculator.h
        webrtc/modules/audio_processing/utility/block_mean_calculator.cc
        webrtc/modules/audio_processing/utility/delay_estimator.h
        webrtc/modules/audio_processing/utility/delay_estimator.cc
        webrtc/modules/audio_processing/utility/delay_estimator_wrapper.h
        webrtc/modules/audio_processing/utility/delay_estimator_wrapper.cc
        webrtc/modules/audio_processing/utility/delay_estimator_internal.h
        webrtc/modules/audio_processing/utility/ooura_fft.h
        webrtc/modules/audio_processing/utility/ooura_fft.cc
        webrtc/modules/audio_processing/utility/ooura_fft_sse2.cc
        webrtc/modules/audio_processing/utility/ooura_fft_tables_common.h)

add_library(webrtc-utility SHARED ${UTIL_FILES})
target_include_directories(webrtc-utility PRIVATE webrtc)
target_compile_definitions(webrtc-utility PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
install(TARGETS webrtc-utility
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")

set(RESAMPLER_FILES
        webrtc/common_audio/resampler/include/resampler.h
        webrtc/common_audio/resampler/include/push_resampler.h
        webrtc/common_audio/resampler/resampler.cc
        webrtc/common_audio/resampler/push_resampler.cc
        webrtc/common_audio/resampler/push_sinc_resampler.h
        webrtc/common_audio/resampler/push_sinc_resampler.cc
        webrtc/common_audio/resampler/sinc_resampler_neon.cc
        webrtc/common_audio/resampler/sinusoidal_linear_chirp_source.h
        webrtc/common_audio/resampler/sinusoidal_linear_chirp_source.cc)

add_library(webrtc-resample SHARED ${UTIL_FILES})
target_include_directories(webrtc-resample PRIVATE webrtc)
target_compile_definitions(webrtc-resample PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
install(TARGETS webrtc-resample
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")

set(DSP_FILES
        webrtc/common_audio/ring_buffer.h
        webrtc/common_audio/ring_buffer.c
        webrtc/common_audio/fir_filter_c.h
        webrtc/common_audio/fir_filter_c.cc
        webrtc/common_audio/fir_filter_sse.h
        webrtc/common_audio/fir_filter_sse.cc
        webrtc/common_audio/fir_filter_factory.h
        webrtc/common_audio/fir_filter_factory.cc
        #webrtc/common_audio/smoothing_filter.h
        #webrtc/common_audio/smoothing_filter.cc
        webrtc/common_audio/window_generator.h
        webrtc/common_audio/window_generator.cc
        webrtc/common_audio/real_fourier_ooura.h
        webrtc/common_audio/real_fourier.cc
        webrtc/common_audio/real_fourier_ooura.h
        webrtc/common_audio/real_fourier_ooura.cc
        webrtc/common_audio/signal_processing/include/signal_processing_library.h
        webrtc/common_audio/signal_processing/include/spl_inl.h
        webrtc/common_audio/signal_processing/auto_corr_to_refl_coef.c
        webrtc/common_audio/signal_processing/auto_corr_to_refl_coef.c
        webrtc/common_audio/signal_processing/complex_bit_reverse.c
        webrtc/common_audio/signal_processing/copy_set_operations.c
        webrtc/common_audio/signal_processing/complex_fft.c
        webrtc/common_audio/signal_processing/cross_correlation.c
        webrtc/common_audio/signal_processing/division_operations.c
        webrtc/common_audio/signal_processing/downsample_fast.c
        webrtc/common_audio/signal_processing/energy.c
        webrtc/common_audio/signal_processing/filter_ar.c
        webrtc/common_audio/signal_processing/get_hanning_window.c
        webrtc/common_audio/signal_processing/get_scaling_square.c
        webrtc/common_audio/signal_processing/ilbc_specific_functions.c
        webrtc/common_audio/signal_processing/levinson_durbin.c
        webrtc/common_audio/signal_processing/lpc_to_refl_coef.c
        webrtc/common_audio/signal_processing/min_max_operations.c
        webrtc/common_audio/signal_processing/randomization_functions.c
        webrtc/common_audio/signal_processing/real_fft.c
        webrtc/common_audio/signal_processing/refl_coef_to_lpc.c
        webrtc/common_audio/signal_processing/resample_fractional.c
        webrtc/common_audio/signal_processing/resample.c
        webrtc/common_audio/signal_processing/resample_48khz.c
        webrtc/common_audio/signal_processing/resample_by_2.c
        webrtc/common_audio/signal_processing/resample_by_2_internal.h
        webrtc/common_audio/signal_processing/resample_by_2_internal.c
        webrtc/common_audio/signal_processing/spl_init.c
        webrtc/common_audio/signal_processing/spl_sqrt.c
        webrtc/common_audio/signal_processing/splitting_filter.c
        webrtc/common_audio/signal_processing/sqrt_of_one_minus_x_squared.c
        webrtc/common_audio/signal_processing/vector_scaling_operations.c
        webrtc/common_audio/signal_processing/dot_product_with_scale.h
        webrtc/common_audio/signal_processing/dot_product_with_scale.cc
        webrtc/common_audio/third_party/fft4g/fft4g.h
        webrtc/common_audio/third_party/fft4g/fft4g.c
        webrtc/common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.c
        webrtc/common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.h
        webrtc/common_audio/include/audio_util.h
        webrtc/common_audio/audio_util.cc)

add_library(webrtc-dsp SHARED ${DSP_FILES})
target_include_directories(webrtc-dsp PRIVATE webrtc)
target_include_directories(webrtc-dsp PUBLIC webrtc/modules/audio_processing)
target_include_directories(webrtc-dsp PUBLIC webrtc/common_audio)
target_include_directories(webrtc-dsp PUBLIC webrtc/common_audio/include)
target_compile_definitions(webrtc-dsp PRIVATE -DWEBRTC_POSIX)
set_target_properties(webrtc-dsp PROPERTIES PUBLIC_HEADER
        webrtc/common_audio/signal_processing/include/signal_processing_library.h)

include(GNUInstallDirs)
install(TARGETS webrtc-dsp
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


set(NS_FILES
        webrtc/modules/audio_processing/ns/defines.h
        webrtc/modules/audio_processing/ns/noise_suppression.h
        webrtc/modules/audio_processing/ns/noise_suppression.c
        webrtc/modules/audio_processing/ns/noise_suppression_x.h
        webrtc/modules/audio_processing/ns/noise_suppression_x.c
        webrtc/modules/audio_processing/ns/ns_core.h
        webrtc/modules/audio_processing/ns/ns_core.c
        webrtc/modules/audio_processing/ns/nsx_core_c.c
        webrtc/modules/audio_processing/ns/nsx_core.h
        webrtc/modules/audio_processing/ns/nsx_core.c
        webrtc/modules/audio_processing/ns/nsx_defines.h
        webrtc/modules/audio_processing/ns/windows_private.h)

add_library(webrtc-noise SHARED ${NS_FILES})
target_include_directories(webrtc-noise PRIVATE webrtc)
target_include_directories(webrtc-noise PUBLIC webrtc/modules/audio_processing/ns)
target_compile_definitions(webrtc-noise PRIVATE -D__native_client__)
target_link_libraries(webrtc-noise PRIVATE webrtc-dsp)
set_target_properties(webrtc-noise PROPERTIES PUBLIC_HEADER webrtc/modules/audio_processing/ns/noise_suppression.h)

include(GNUInstallDirs)
install(TARGETS webrtc-noise
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


include(GNUInstallDirs)
install(TARGETS webrtc-system
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")

set(AGC_FILES
        webrtc/modules/audio_processing/agc/legacy/gain_control.h
        webrtc/modules/audio_processing/agc/legacy/digital_agc.h
        webrtc/modules/audio_processing/agc/legacy/digital_agc.c
        webrtc/modules/audio_processing/agc/legacy/analog_agc.h
        webrtc/modules/audio_processing/agc/legacy/analog_agc.c)

add_library(webrtc-agc SHARED ${AGC_FILES})
target_include_directories(webrtc-agc PRIVATE webrtc)
target_include_directories(webrtc-agc PUBLIC webrtc/modules/audio_processing/agc/legacy)
target_compile_definitions(webrtc-agc PRIVATE -D__native_client__)
target_link_libraries(webrtc-agc PRIVATE webrtc-dsp webrtc-system)
set_target_properties(webrtc-agc PROPERTIES PUBLIC_HEADER webrtc/modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-agc
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


set(AEC_FILES
        webrtc/modules/audio_processing/aec/aec_core.h
        webrtc/modules/audio_processing/aec/aec_core.cc
        webrtc/modules/audio_processing/aec/aec_core_sse2.cc
        webrtc/modules/audio_processing/aec/aec_common.h
        webrtc/modules/audio_processing/aec/aec_core_optimized_methods.h
        webrtc/modules/audio_processing/aec/aec_resampler.cc
        webrtc/modules/audio_processing/aec/aec_resampler.h
        webrtc/modules/audio_processing/aec/echo_cancellation.h
        webrtc/modules/audio_processing/aec/echo_cancellation.cc
)

add_library(webrtc-aec SHARED ${AEC_FILES})
target_include_directories(webrtc-aec PRIVATE webrtc)
target_include_directories(webrtc-aec PUBLIC webrtc/modules/audio_processing/agc/legacy)
target_compile_definitions(webrtc-aec PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
target_link_libraries(webrtc-aec PRIVATE webrtc-dsp webrtc-system webrtc-utility)
set_target_properties(webrtc-aec PROPERTIES PUBLIC_HEADER webrtc/modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-aec
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


set(RNN_VAD_FILES
        webrtc/third_party/rnnoise/src/kiss_fft.h
        webrtc/third_party/rnnoise/src/kiss_fft.cc
        webrtc/third_party/rnnoise/src/rnn_activations.h
        webrtc/third_party/rnnoise/src/rnn_vad_weights.h
        webrtc/third_party/rnnoise/src/rnn_vad_weights.cc
        webrtc/modules/audio_processing/agc2/biquad_filter.h
        webrtc/modules/audio_processing/agc2/biquad_filter.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/common.h
        webrtc/modules/audio_processing/agc2/rnn_vad/features_extraction.h
        webrtc/modules/audio_processing/agc2/rnn_vad/features_extraction.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/fft_util.h
        webrtc/modules/audio_processing/agc2/rnn_vad/fft_util.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/lp_residual.h
        webrtc/modules/audio_processing/agc2/rnn_vad/lp_residual.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/pitch_info.h
        webrtc/modules/audio_processing/agc2/rnn_vad/pitch_search.h
        webrtc/modules/audio_processing/agc2/rnn_vad/pitch_search.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/pitch_search_internal.h
        webrtc/modules/audio_processing/agc2/rnn_vad/pitch_search_internal.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/ring_buffer.h
        webrtc/modules/audio_processing/agc2/rnn_vad/rnn.h
        webrtc/modules/audio_processing/agc2/rnn_vad/rnn.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/sequence_buffer.h
        webrtc/modules/audio_processing/agc2/rnn_vad/spectral_features.h
        webrtc/modules/audio_processing/agc2/rnn_vad/spectral_features.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/spectral_features_internal.h
        webrtc/modules/audio_processing/agc2/rnn_vad/spectral_features_internal.cc
        webrtc/modules/audio_processing/agc2/rnn_vad/symmetric_matrix_buffer.h)

add_library(webrtc-rnn-vad SHARED ${RNN_VAD_FILES})
target_include_directories(webrtc-rnn-vad PRIVATE webrtc)
target_include_directories(webrtc-rnn-vad PUBLIC webrtc/modules/audio_processing/agc2)
target_include_directories(webrtc-rnn-vad PRIVATE webrtc/modules)
target_compile_definitions(webrtc-rnn-vad PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
target_link_libraries(webrtc-rnn-vad PRIVATE webrtc-dsp webrtc-system webrtc-utility webrtc-resample)
set_target_properties(webrtc-rnn-vad PROPERTIES PUBLIC_HEADER webrtc/modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-rnn-vad
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")

set(ECHO_FILES
        webrtc/modules/audio_processing/echo_detector/circular_buffer.h
        webrtc/modules/audio_processing/echo_detector/circular_buffer.cc
        webrtc/modules/audio_processing/echo_detector/moving_max.h
        webrtc/modules/audio_processing/echo_detector/moving_max.cc
        webrtc/modules/audio_processing/echo_detector/mean_variance_estimator.h
        webrtc/modules/audio_processing/echo_detector/mean_variance_estimator.cc
        webrtc/modules/audio_processing/echo_detector/normalized_covariance_estimator.h
        webrtc/modules/audio_processing/echo_detector/normalized_covariance_estimator.cc)

add_library(webrtc-red SHARED ${ECHO_FILES})
target_include_directories(webrtc-red PRIVATE webrtc)
target_include_directories(webrtc-red PUBLIC webrtc/modules/audio_processing/echo_detector)
target_compile_definitions(webrtc-red PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
target_link_libraries(webrtc-red PRIVATE webrtc-dsp webrtc-system webrtc-utility)
set_target_properties(webrtc-red PROPERTIES PUBLIC_HEADER webrtc/modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-red
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")

