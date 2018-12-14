#include <recorder.hpp>
#include <encoder.hpp>

#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>

using namespace score;
namespace po = boost::program_options;

int main(int ac, char* av[]) {
    Recorder::Initialize();

    std::string filename;
    std::size_t duration;
    std::int32_t sample_rate;
    std::int32_t device_index;
    std::int32_t channels;

    po::options_description desc("Recording options");
    desc.add_options()
            ("help, h", "Help message")
            ("file, f", po::value<std::string>(&filename)->required(), "File path of the recorded audio file")
            ("duration, d", po::value<std::size_t >(&duration)->required(), "Duration of the recording in milliseconds.")
            ("sample-rate, sr", po::value<std::int32_t>(&sample_rate)->default_value(16000), "Sampling frequency in Hz. Default: 16000Hz")
            ("channels, ch", po::value<std::int32_t >(&channels)->default_value(1), "Number of channels. Default: Mono (1)")
            ("index, i", po::value<std::int32_t >(&device_index)->default_value(Recorder::DefaultInputDevice()), "Device index. Default: default system input device.");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    po::notify(vm);

    auto recorder = std::make_unique<Recorder>(sample_rate, channels, device_index, 0.01 * sample_rate);
    auto encoder = std::make_unique<Encoder>(filename, sample_rate, channels);
    volatile auto iterations = static_cast<std::uint64_t >(duration) / 10;
    recorder->setOnRecordingStarted([](){ std::cout << "Recording started" << std::endl;  });
    recorder->setOnRecordingStopped([]() {  std::cout << "Recording stopped" << std::endl;  });
    recorder->setOnProcessingBufferReady([&](AudioBuffer& recorded) {
        encoder->process(recorded);
        --iterations;
    });
    recorder->record();

    while (iterations) {

    }
    return 0;
}