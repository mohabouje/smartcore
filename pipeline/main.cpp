#include <recorder.hpp>
#include <acoustic_echo_canceller.hpp>
#include <noise_suppression.hpp>
#include <rnn_noise_suppression.hpp>
#include <resample.hpp>
#include <iostream>
#include <chrono>

using namespace score;
int main() {

    Recorder::Initialize();

    const auto sample_rate = 16000;
    const auto device_index = Recorder::DefaultInputDevice();
    const auto channels = 1;
    const auto frame_per_buffer = static_cast<std::size_t >(0.01 * sample_rate);


    auto recorder = std::make_unique<Recorder>(sample_rate,channels,
            device_index, frame_per_buffer);
    auto denoiser = std::make_unique<NoiseSuppression>(sample_rate, channels, NoiseSuppression::Medium);
    auto rnn_denoiser = std::make_unique<DeepNoiseSuppression>(channels);

    auto upsampler = std::make_unique<ReSampler>(channels, sample_rate,
            DeepNoiseSuppression::DefaultSampleRate, ReSampler::Quality::HighQuality);

    auto downsampler = std::make_unique<ReSampler>(channels, DeepNoiseSuppression::DefaultSampleRate,
            sample_rate, ReSampler::Quality::HighQuality);


    recorder->setOnRecordingStarted([](){
        std::cout << "Recording started" << std::endl;
    });
    recorder->setOnRecordingStopped([]() {
        std::cout << "Recording stopped" << std::endl;
    });

    AudioBuffer output(sample_rate, channels, frame_per_buffer);
    AudioBuffer resampled(DeepNoiseSuppression::DefaultSampleRate, channels, DeepNoiseSuppression::DefaultBufferSize);
    recorder->setOnProcessingBufferReady([&](AudioBuffer& recorded) {
        denoiser->process(recorded, output);

        upsampler->process(output, resampled);
        rnn_denoiser->process(resampled, resampled);
        downsampler->process(resampled, output);
    });
    recorder->record();

    while (true) {

    }
    return 0;
}