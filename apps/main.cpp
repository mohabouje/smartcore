#include <recorder.hpp>
#include <vad.hpp>
#include <low_cut_filter.hpp>
#include <noise_suppression.hpp>
#include <resample.hpp>
#include <downmix.hpp>
#include <encoder.hpp>
#include <gain.hpp>

#include <iostream>
#include <chrono>

using namespace score;
int main() {

    Recorder::Initialize();

    const auto sample_rate = 48000;
    const auto device_index = Recorder::DefaultInputDevice();
    const auto channels = 1;
    const auto frame_per_buffer = static_cast<std::size_t >(0.01 * sample_rate);


    auto recorder = std::make_unique<Recorder>(sample_rate, channels, device_index, frame_per_buffer);
    auto vad = std::make_unique<VAD>(sample_rate, VAD::Aggressive);
    auto low_cut = std::make_unique<LowCutFilter>(sample_rate, channels);
    auto denoiser = std::make_unique<NoiseSuppression>(sample_rate, channels, NoiseSuppression::Aggressive);
    auto downmix = std::make_unique<DownMix>();

    auto upsampler = std::make_unique<ReSampler>(channels, sample_rate, 48000, ReSampler::Quality::HighQuality);
    auto downsampler = std::make_unique<ReSampler>(channels, 48000, sample_rate, ReSampler::Quality::HighQuality);
    auto gain = std::make_unique<Gain>(3);

    auto e_original = std::make_unique<Encoder>("original.wav", sample_rate, channels);
    auto e_clean_webrtc = std::make_unique<Encoder>("clean_webrtc.wav", sample_rate, channels);
    auto e_upsampled = std::make_unique<Encoder>("upsampled_original.wav", 48000, channels);
    auto e_downsampled = std::make_unique<Encoder>("downsampled_original.wav", sample_rate, channels);
    auto e_clean_rnn = std::make_unique<Encoder>("clean_rnn.wav", 48000, channels);

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
        e_clean_rnn->process(recorded);
        --iteration;
    });
    recorder->record();

    while (iteration) {

    }
    return 0;
}