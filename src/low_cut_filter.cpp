#include "low_cut_filter.hpp"
#include <edsp/filter/biquad.hpp>

using namespace score;


struct LowCutFilter::Pimpl {

    Pimpl(std::int32_t sample_rate, std::int8_t channels) :
        sample_rate_(sample_rate),
        channels_(channels),
        filters_(channels, edsp::filter::biquad<float>(1.0f, -1.0f, 0.0f, 1.0f, -0.95f, 0.0)) {

    }

    void process(const score::AudioBuffer &input, score::AudioBuffer &output) {

        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected "
                                        + std::to_string(sample_rate_) + " Hz");
        }

        if (channels_ != input.channels()) {
            throw std::invalid_argument("The LowCutOff is configure to work with "
                                        + std::to_string(channels_) + " channels.");
        }

        output.setSampleRate(input.sampleRate());
        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < channels_; ++i) {
            filters_[i].filter(input.channel(i), input.channel(i) + input.framesPerChannel(), output.channel(i));
        }
    }
private:
    std::int8_t channels_;
    std::int32_t sample_rate_;
    std::vector<edsp::filter::biquad<float>> filters_;
};


score::LowCutFilter::LowCutFilter(std::int32_t sample_rate, std::int8_t channels) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels)){

}

void score::LowCutFilter::process(const score::AudioBuffer &input, score::AudioBuffer &output) {
    pimpl_->process(input, output);
}

score::LowCutFilter::~LowCutFilter() = default;
