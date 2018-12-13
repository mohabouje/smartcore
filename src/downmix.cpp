#include "downmix.hpp"

#include <edsp/algorithm/amplifier.hpp>


using namespace score;

struct DownMix::Pimpl {

    explicit Pimpl() {

    }



    void process(const AudioBuffer &input, AudioBuffer& output) {
        const auto channels = input.channels();
        const auto frame_length = input.framesPerChannel();
        output.setSampleRate(input.sampleRate());
        output.resize(1, frame_length);
        auto* out = output.channel(0);
        for (auto i = 0ul; i < channels; ++i) {
            std::transform(out, out + frame_length, input.channel(i), out, std::plus<>());
        }

        edsp::amplifier(out, out + frame_length, out, 1.f / static_cast<float>(channels));
    }


    Matrix<std::int16_t> bands_{};
    std::array<std::int16_t *, Bands::NumberBands> bands_ptr_{};
};

void score::DownMix::process(const AudioBuffer &input, AudioBuffer &output) {
    pimpl_->process(input, output);
}

score::DownMix::DownMix() : pimpl_(std::make_unique<Pimpl>()) {

}



score::DownMix::~DownMix() = default;
