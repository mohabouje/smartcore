#include "resample.hpp"
#include <speex/speex_resampler.h>

using namespace score;

struct ReSampler::Pimpl {

    Pimpl(std::uint8_t channels, std::uint32_t input_rate, std::uint32_t output_rate, Quality quality) :
            channels_(channels),
            input_rate_(input_rate),
            output_rate_(output_rate),
            ratio_(static_cast<float>(output_rate) / static_cast<float>(input_rate))
    {

        quality_ = static_cast<std::underlying_type<Quality >::type>(quality);
        state_                   = speex_resampler_init(channels_, input_rate_, output_rate_, quality_, &error_);
        ensure_no_error();
    }

    ~Pimpl() {
        speex_resampler_destroy(state_);
    }

    void process(const AudioBuffer& input, AudioBuffer& output) {

        if (input.channels() != channels_) {
            throw std::invalid_argument("Expected an input frame with " + std::to_string(channels_) + " channels.");
        }

        if (input.sampleRate() != input_rate_) {
            throw std::runtime_error("Expecting an input buffer of "
                                     + std::to_string(input_rate_) + " Hz.");
        }

        const auto expected_output_size = static_cast<size_t>(input.framesPerChannel() * ratio_);
        auto input_size = static_cast<uint32_t>(input.framesPerChannel() * input.channels()),
            output_size = static_cast<uint32_t>(input.channels() * expected_output_size);
        input_.resize(input_size);
        output_.resize(output_size);

        error_ = speex_resampler_process_interleaved_int(state_, input_.data(), &input_size,
                output_.data(), &output_size);
        ensure_no_error();

        output.setSampleRate(output_rate_);
        output.fromInterleave(input.channels(), expected_output_size, output_.data());
    }

    void reset() {
        speex_resampler_reset_mem(state_);
    }

    float ratio() const {
        return ratio_;
    }

    void setRatio(float ratio) {
        ratio_ = ratio;
        ensure_no_error();
    }

    void ensure_no_error() {
        if (error_ != 0) {
            throw std::runtime_error("Error while running re-sampling:" + std::string(speex_resampler_strerror(error_)));
        }
    }

    int quality() const {
        return quality_;
    }

    SpeexResamplerState* state_{nullptr};
    std::vector<std::int16_t> input_;
    std::vector<std::int16_t> output_;
    std::uint32_t input_rate_;
    std::uint32_t output_rate_;
    std::uint8_t channels_{0};
    int error_{0};
    int quality_{};
    float ratio_{1};
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

void score::ReSampler::setRatio(float ratio) {
    pimpl_->setRatio(ratio);
}

ReSampler::Quality score::ReSampler::quality() const {
    return static_cast<Quality>(pimpl_->quality());
}
