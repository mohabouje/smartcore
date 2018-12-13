#include <recorder.hpp>
#include <vad.hpp>
#include <low_cut_filter.hpp>
#include <noise_suppression.hpp>
#include <rnn_noise_suppression.hpp>
#include <resample.hpp>
#include <downmix.hpp>
#include <encoder.hpp>

#include <iostream>
#include <chrono>

using namespace score;
int main() {

    Recorder::Initialize();

    const auto sample_rate = 8000;
    const auto device_index = Recorder::DefaultInputDevice();
    const auto channels = 1;
    const auto frame_per_buffer = static_cast<std::size_t >(0.01 * sample_rate);


    auto recorder = std::make_unique<Recorder>(sample_rate, channels, device_index, frame_per_buffer);
    auto vad = std::make_unique<VAD>(sample_rate, VAD::Aggressive);
    auto low_cut = std::make_unique<LowCutFilter>(sample_rate, channels);
    auto denoiser = std::make_unique<NoiseSuppression>(sample_rate, channels, NoiseSuppression::Aggressive);
    auto rnn_denoiser = std::make_unique<DeepNoiseSuppression>(channels);
    auto downmix = std::make_unique<DownMix>();

    auto upsampler = std::make_unique<ReSampler>(channels, sample_rate, 48000, ReSampler::Quality::HighQuality);
    auto downsampler = std::make_unique<ReSampler>(channels, 48000, sample_rate, ReSampler::Quality::HighQuality);

    auto e_original = std::make_unique<Encoder>("original.wav", sample_rate, channels);
    auto e_clean_webrtc = std::make_unique<Encoder>("clean_webrtc.wav", sample_rate, channels);
    auto e_clean_rnn = std::make_unique<Encoder>("clean_rnn.wav", sample_rate, channels);

    recorder->setOnRecordingStarted([](){
        std::cout << "Recording started" << std::endl;
    });
    recorder->setOnRecordingStopped([]() {
        std::cout << "Recording stopped" << std::endl;
    });

    AudioBuffer output, upsampled, upsampled_clean;
    volatile auto iteration = 1000;
    recorder->setOnProcessingBufferReady([&](AudioBuffer& recorded) {
        e_original->process(recorded);
        denoiser->process(recorded, output);
        e_clean_webrtc->process(output);

        //upsampler->process(recorded, upsampled);
        //rnn_denoiser->process(upsampled, upsampled_clean);
        //downsampler->process(upsampled_clean, output);
        //e_clean_rnn->process(upsampled);

        --iteration;
    });
    recorder->record();

    while (iteration) {

    }
    return 0;
}