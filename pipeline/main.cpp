#include <recorder.hpp>
#include <vad.hpp>
#include <low_cut_filter.hpp>
#include <noise_suppression.hpp>
#include <downmix.hpp>
#include <encoder.hpp>

#include <iostream>
#include <chrono>

using namespace score;
int main() {

    Recorder::Initialize();

    const auto sample_rate = 16000;
    const auto device_index = Recorder::DefaultInputDevice();
    const auto channels = 2;
    const auto frame_per_buffer = static_cast<std::size_t >(0.01 * sample_rate);


    auto recorder = std::make_unique<Recorder>(sample_rate, channels, device_index, frame_per_buffer);
    auto vad = std::make_unique<VAD>(sample_rate, VAD::Aggressive);
    auto low_cut = std::make_unique<LowCutFilter>(sample_rate, channels);
    auto denoiser = std::make_unique<NoiseSuppression>(sample_rate, channels, NoiseSuppression::Aggressive);
    auto downmix = std::make_unique<DownMix>(Bands::Band0To8kHz);
    auto encoder = std::make_unique<Encoder>("example.wav", sample_rate, channels);

    recorder->setOnRecordingStarted([](){
        std::cout << "Recording started" << std::endl;
    });
    recorder->setOnRecordingStopped([]() {
        std::cout << "Recording stopped" << std::endl;
    });

    AudioBuffer output;
    recorder->setOnProcessingBufferReady([&](AudioBuffer& recorded) {
        low_cut->process(recorded, recorded);
        denoiser->process(recorded, recorded);
        downmix->process(recorded, output);
        std::cout << "Is there any voice ? " << vad->process(output) << std::endl;
    });
    recorder->record();

    while (true) {}
    return 0;
}