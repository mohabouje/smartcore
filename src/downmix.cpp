#include "downmix.hpp"
#include "band_extractor.hpp"

using namespace score;

struct DownMix::Pimpl {

    explicit Pimpl(Bands band) : band_(band) {

    }

    void resize(std::size_t frame_length) {
        bands_.resize(Bands::NumberBands, frame_length);
        for (auto i = 0ul; i < Bands::NumberBands; ++i) {
            bands_ptr_[i] = bands_.row(i).data();
            bands_ptr_[i] = bands_.row(i).data();
        }
    }

    void process(const AudioBuffer &input, AudioBuffer& output) {
        const auto channels = input.channels();
        const auto frame_length = input.framesPerChannel();
        resize(frame_length);

        output.setSampleRate(input.sampleRate());
        output.resize(1, frame_length);
        auto* out = output.channel(0);
        for (auto i = 0ul; i < channels; ++i) {
            band_extractor_.process(input.channel(i), frame_length, bands_);
            for (auto j = 0ul; j < frame_length; ++j) {
                out[j] += bands_(band_, j);
            }
        }

        for (auto j = 0ul; j < frame_length; ++j) {
            out[j] /= channels;
        }
    }


    Bands band_{};
    BandExtractor band_extractor_{};
    Matrix<std::int16_t> bands_{};
    std::array<std::int16_t *, Bands::NumberBands> bands_ptr_{};
};

void score::DownMix::process(const AudioBuffer &input, AudioBuffer &output) {
    pimpl_->process(input, output);
}

score::DownMix::DownMix(Bands band) : pimpl_(std::make_unique<Pimpl>(band)) {

}

Bands score::DownMix::band() const {
    return pimpl_->band_;
}

void score::DownMix::setBand(Bands band) {
    pimpl_->band_ = band;
}

score::DownMix::~DownMix() = default;
