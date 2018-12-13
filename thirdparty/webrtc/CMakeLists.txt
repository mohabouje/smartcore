cmake_minimum_required(VERSION 3.4)
project(webrtc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SYSTEM_FILES
        system_wrappers/include/metrics.h
        system_wrappers/source/metrics.cc
        system_wrappers/include/cpu_features_wrapper.h
        system_wrappers/source/cpu_features.cc
        rtc_base/memory/aligned_malloc.h
        rtc_base/memory/aligned_malloc.cc
        rtc_base/criticalsection.h
        rtc_base/criticalsection.cc
        rtc_base/platform_thread_types.h
        rtc_base/platform_thread_types.cc
        rtc_base/checks.h
        rtc_base/checks.cc)

add_library(webrtc-system SHARED ${SYSTEM_FILES})
target_include_directories(webrtc-system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(webrtc-system PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)

set(UTIL_FILES
        modules/audio_processing/utility/block_mean_calculator.h
        modules/audio_processing/utility/block_mean_calculator.cc
        modules/audio_processing/utility/delay_estimator.h
        modules/audio_processing/utility/delay_estimator.cc
        modules/audio_processing/utility/delay_estimator_wrapper.h
        modules/audio_processing/utility/delay_estimator_wrapper.cc
        modules/audio_processing/utility/delay_estimator_internal.h
        modules/audio_processing/utility/ooura_fft.h
        modules/audio_processing/utility/ooura_fft.cc
        modules/audio_processing/utility/ooura_fft_sse2.cc
        modules/audio_processing/utility/ooura_fft_tables_common.h)

add_library(webrtc-utility SHARED ${UTIL_FILES})
target_include_directories(webrtc-utility PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(webrtc-utility PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)

set(RESAMPLER_FILES
        common_audio/resampler/include/resampler.h
        common_audio/resampler/include/push_resampler.h
        common_audio/resampler/resampler.cc
        common_audio/resampler/push_resampler.cc
        common_audio/resampler/push_sinc_resampler.h
        common_audio/resampler/push_sinc_resampler.cc
        common_audio/resampler/sinc_resampler_neon.cc
        common_audio/resampler/sinusoidal_linear_chirp_source.h
        common_audio/resampler/sinusoidal_linear_chirp_source.cc)

add_library(webrtc-resample SHARED ${UTIL_FILES})
target_include_directories(webrtc-resample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(webrtc-resample PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)


set(DSP_FILES
        common_audio/ring_buffer.h
        common_audio/ring_buffer.c
        common_audio/fir_filter_c.h
        common_audio/fir_filter_c.cc
        common_audio/fir_filter_sse.h
        common_audio/fir_filter_sse.cc
        common_audio/fir_filter_factory.h
        common_audio/fir_filter_factory.cc
        #common_audio/smoothing_filter.h
        #common_audio/smoothing_filter.cc
        common_audio/window_generator.h
        common_audio/window_generator.cc
        common_audio/real_fourier_ooura.h
        common_audio/real_fourier.cc
        common_audio/real_fourier_ooura.h
        common_audio/real_fourier_ooura.cc
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
        modules/audio_processing/ns/windows_private.h)

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


include(GNUInstallDirs)
install(TARGETS webrtc-system
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
target_link_libraries(webrtc-agc PRIVATE webrtc-dsp webrtc-system)
set_target_properties(webrtc-agc PROPERTIES PUBLIC_HEADER modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-agc
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


set(AEC_FILES
        modules/audio_processing/aec/aec_core.h
        modules/audio_processing/aec/aec_core.cc
        modules/audio_processing/aec/aec_core_sse2.cc
        modules/audio_processing/aec/aec_common.h
        modules/audio_processing/aec/aec_core_optimized_methods.h
        modules/audio_processing/aec/aec_resampler.cc
        modules/audio_processing/aec/aec_resampler.h
        modules/audio_processing/aec/echo_cancellation.h
        modules/audio_processing/aec/echo_cancellation.cc
)

add_library(webrtc-aec SHARED ${AEC_FILES})
target_include_directories(webrtc-aec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(webrtc-aec PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_processing/agc/legacy/)
target_compile_definitions(webrtc-aec PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
target_link_libraries(webrtc-aec PRIVATE webrtc-dsp webrtc-system webrtc-utility)
set_target_properties(webrtc-aec PROPERTIES PUBLIC_HEADER modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-aec
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")


set(RNN_VAD_FILES
        third_party/rnnoise/src/kiss_fft.h
        third_party/rnnoise/src/kiss_fft.cc
        third_party/rnnoise/src/rnn_activations.h
        third_party/rnnoise/src/rnn_vad_weights.h
        third_party/rnnoise/src/rnn_vad_weights.cc
        modules/audio_processing/agc2/biquad_filter.h
        modules/audio_processing/agc2/biquad_filter.cc
        modules/audio_processing/agc2/rnn_vad/common.h
        modules/audio_processing/agc2/rnn_vad/features_extraction.h
        modules/audio_processing/agc2/rnn_vad/features_extraction.cc
        modules/audio_processing/agc2/rnn_vad/fft_util.h
        modules/audio_processing/agc2/rnn_vad/fft_util.cc
        modules/audio_processing/agc2/rnn_vad/lp_residual.h
        modules/audio_processing/agc2/rnn_vad/lp_residual.cc
        modules/audio_processing/agc2/rnn_vad/pitch_info.h
        modules/audio_processing/agc2/rnn_vad/pitch_search.h
        modules/audio_processing/agc2/rnn_vad/pitch_search.cc
        modules/audio_processing/agc2/rnn_vad/pitch_search_internal.h
        modules/audio_processing/agc2/rnn_vad/pitch_search_internal.cc
        modules/audio_processing/agc2/rnn_vad/ring_buffer.h
        modules/audio_processing/agc2/rnn_vad/rnn.h
        modules/audio_processing/agc2/rnn_vad/rnn.cc
        modules/audio_processing/agc2/rnn_vad/sequence_buffer.h
        modules/audio_processing/agc2/rnn_vad/spectral_features.h
        modules/audio_processing/agc2/rnn_vad/spectral_features.cc
        modules/audio_processing/agc2/rnn_vad/spectral_features_internal.h
        modules/audio_processing/agc2/rnn_vad/spectral_features_internal.cc
        modules/audio_processing/agc2/rnn_vad/symmetric_matrix_buffer.h)

add_library(webrtc-rnn-vad SHARED ${RNN_VAD_FILES})
target_include_directories(webrtc-rnn-vad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(webrtc-rnn-vad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_processing/agc2/)
target_include_directories(webrtc-rnn-vad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modules/)
target_compile_definitions(webrtc-rnn-vad PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
target_link_libraries(webrtc-rnn-vad PRIVATE webrtc-dsp webrtc-system webrtc-utility webrtc-resample)
set_target_properties(webrtc-rnn-vad PROPERTIES PUBLIC_HEADER modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-rnn-vad
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")

set(ECHO_FILES
        modules/audio_processing/echo_detector/circular_buffer.h
        modules/audio_processing/echo_detector/circular_buffer.cc
        modules/audio_processing/echo_detector/moving_max.h
        modules/audio_processing/echo_detector/moving_max.cc
        modules/audio_processing/echo_detector/mean_variance_estimator.h
        modules/audio_processing/echo_detector/mean_variance_estimator.cc
        modules/audio_processing/echo_detector/normalized_covariance_estimator.h
        modules/audio_processing/echo_detector/normalized_covariance_estimator.cc)

add_library(webrtc-red SHARED ${AEC_FILES})
target_include_directories(webrtc-red PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(webrtc-red PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_processing/agc/legacy/)
target_compile_definitions(webrtc-red PRIVATE -D__native_client__ -DWEBRTC_APM_DEBUG_DUMP=0 -DWEBRTC_POSIX -DWEBRTC_LINUX)
target_link_libraries(webrtc-red PRIVATE webrtc-dsp webrtc-system webrtc-utility)
set_target_properties(webrtc-red PROPERTIES PUBLIC_HEADER modules/audio_processing/agc/legacy/gain_control.h)

include(GNUInstallDirs)
install(TARGETS webrtc-red
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/smartmeet/core/")

