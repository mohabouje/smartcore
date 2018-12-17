#include "delay.hpp"
using namespace score;

struct Delay::Pimpl {

    Pimpl(std::int32_t sample_rate, std::int32_t delay) :
        sample_rate_(sample_rate),
        delay_samples_(static_cast<int32_t>(sample_rate * static_cast<float>(delay) / 1000.f)) {

    }

    void delaySequence(const float *in_data, std::size_t num, int delay, float *out_data) {
        for (int i = 0; i < num; i++) {
            if (i - delay >= 0 && i - delay < num) {
                out_data[i] = in_data[i - delay];
            } else {
                out_data[i] = 0;
            }
        }
    }

    void process(const score::AudioBuffer &input, score::AudioBuffer &output) {
        if (sample_rate_ != input.sampleRate()) {
            throw std::invalid_argument("The block is configured to work with "
                                        + std::to_string(sample_rate_) + " Hz.");
        }

        output.setSampleRate(input.sampleRate());
        output.resize(input.channels(), input.framesPerChannel());

        for (auto i = 0ul; i < input.channels(); ++i) {
            delaySequence(input.channel(i), input.framesPerChannel(), delay_samples_, output.channel(i));
        }
    }

private:
    std::int32_t delay_samples_;
    std::int32_t sample_rate_;
};

score::Delay::~Delay() {

}

score::Delay::Delay(std::int32_t sample_rate, std::int32_t delay) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, delay)) {

}

void score::Delay::process(const score::AudioBuffer &input, score::AudioBuffer &output) {
    pimpl_->process(input, output);
}
