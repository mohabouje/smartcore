#include "gain.hpp"
#include "utils.hpp"

#include <edsp/algorithm/amplifier.hpp>

using namespace score;

struct Gain::Pimpl {

    Pimpl(float gain) :
            current_gain_(gain),
            last_gain_(gain) {

    }


    inline bool gainCloseToOne(float gain_factor) {
        return 1.f - 1.f / MaxFloatS16 <= gain_factor && gain_factor <= 1.f + 1.f / MaxFloatS16;
    }


    void process(const AudioBuffer& input, AudioBuffer& output) {

        // Do not modify the signal.
        if (last_gain_ == current_gain_ && gainCloseToOne(current_gain_)) {
            input.copyTo(output);
            return;
        }

        output.setSampleRate(input.sampleRate());
        output.resize(input.channels(), input.framesPerChannel());

        // Gain is constant and different from 1.
        if (last_gain_ == current_gain_) {
            for (auto ch = 0ul; ch < input.channels(); ++ch) {
                auto* out = output.channel(ch);
                auto* in = input.channel(ch);
                edsp::amplifier(in, in + input.framesPerChannel(), out, current_gain_);
            }
            return;
        }

        // The gain changes. We have to change slowly to avoid discontinuities.
        const auto inverse_samples_per_channel = 1.0f / static_cast<float>(input.channels());
        const float increment = (current_gain_ - last_gain_) * inverse_samples_per_channel;
        for (auto ch = 0ul; ch < input.channels(); ++ch) {
            auto* out = output.channel(ch);
            auto* in = input.channel(ch);
            float gain = last_gain_;
            for (auto i = 0ul; i < input.framesPerChannel(); ++i) {
                out[i] = in[i] * gain;
                gain += increment;
            }
        }

        last_gain_ = current_gain_;
    }


    float current_gain_;
    float last_gain_;
};

void Gain::setGainFactor(float factor) {
    pimpl_->current_gain_  = factor;
}

float Gain::gainFactor() const {
    return pimpl_->current_gain_;
}

score::Gain::Gain(float gain):
    pimpl_(std::make_unique<Pimpl>(gain)) {

}

void score::Gain::process(const AudioBuffer &input, AudioBuffer &output) {
    return pimpl_->process(input, output);
}

score::Gain::~Gain() = default;
