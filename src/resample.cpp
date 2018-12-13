#include "resample.hpp"
#include <edsp/io/resampler.hpp>

using namespace score;

struct ReSampler::Pimpl {

    Pimpl(std::uint8_t channels, std::uint32_t input_rate, std::uint32_t output_rate, Quality quality) :
            channels_(channels),
            input_rate_(input_rate),
            output_rate_(output_rate),
            ratio_(static_cast<float>(output_rate) / static_cast<float>(input_rate)),
            resampler_(channels, static_cast<edsp::io::resample_quality>(static_cast<std::underlying_type<Quality>::type>(quality)), ratio_)
    {

        quality_ = static_cast<std::underlying_type<Quality >::type>(quality);
        ensure_no_error();
    }

    ~Pimpl() {

    }

    void process(const AudioBuffer& input, AudioBuffer& output) {

        if (input.channels() != channels_) {
            throw std::invalid_argument("Expected an input frame with " + std::to_string(channels_) + " channels.");
        }

        if (input.sampleRate() != input_rate_) {
            throw std::runtime_error("Expecting an input buffer of "
                                     + std::to_string(input_rate_) + " Hz.");
        }

        // TODO: resample the data!
        const auto expected_output_size = static_cast<size_t>(input.framesPerChannel() * ratio_);
        output.setSampleRate(output_rate_);
        output.resize(channels_, expected_output_size);
    }

    void reset() {
        resampler_.reset();
    }

    float ratio() const {
        return ratio_;
    }

    void ensure_no_error() {
        if (resampler_.error() != 0) {
            throw std::runtime_error("Error while running re-sampling:" + std::string(resampler_.error_string()));
        }
    }

    int quality() const {
        return quality_;
    }


    std::vector<float> input_;
    std::vector<float> output_;

    std::uint32_t input_rate_;
    std::uint32_t output_rate_;
    std::uint8_t channels_{0};
    int quality_{};
    float ratio_{1};
    edsp::io::resampler<float> resampler_;
};

ReSampler::ReSampler(std::uint8_t channels, std::uint32_t input_rate, std::uint32_t output_rate, Quality quality) :
    pimpl_(std::make_unique<Pimpl>(channels, input_rate, output_rate, quality)) {

}

ReSampler::~ReSampler() = default;

void ReSampler::reset() {
    pimpl_->reset();
}


void ReSampler::process(const AudioBuffer& input, AudioBuffer& output) {
    pimpl_->process(input, output);
}

float ReSampler::ratio() const {
    return pimpl_->ratio_;
}

ReSampler::Quality score::ReSampler::quality() const {
    return static_cast<Quality>(pimpl_->quality());
}
