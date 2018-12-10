#include <recorder.hpp>
#include <acoustic_echo_canceller.hpp>
#include <iostream>

using namespace score;
int main() {

    Recorder::Initialize();

    const auto sample_rate = 8000;
    const auto device_index = Recorder::DefaultInputDevice();
    const auto channels = 1;
    const auto frame_per_buffer = 0.02 * sample_rate;
    const auto filter_legnth = 0.2 * sample_rate;


    auto recorder = std::make_unique<Recorder>(sample_rate,channels,
            device_index, frame_per_buffer);

    recorder->setOnRecordingStarted([](){
        std::cout << "Recording started" << std::endl;
    });
    recorder->setOnRecordingStopped([]() {
        std::cout << "Recording stopped" << std::endl;
    });
    recorder->setOnProcessingBufferReady([&](AudioBuffer& recorded) {
    });
    recorder->record();

    while (true) {

    }
    return 0;
}