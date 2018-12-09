
#include <mixed_low_pass.hpp>

#include "mixed_low_pass.hpp"

using namespace score;

struct MixedLowPass::Pimpl {

    Pimpl(float cut_off) : cut_off_(cut_off) {

    }

    void filter(const std::vector<float>& input, std::vector<float>& output) {
        // TODO: filter the signal
    }

    void process(const std::vector<std::vector<float>>& inputs, std::vector<float>& output) {
        if (inputs.empty()) {
            return;
        }

        const auto channels = inputs.size();
        const auto samples = inputs.front().size();

        output.resize(samples);
        filtered_.resize(samples);
        for (auto i = 0ul; i < channels; ++i) {
            filter(inputs[i], filtered_);
            for (auto j = 0ul; j < samples; ++j) {
                output[j] += (filtered_[j] / static_cast<float>(channels));
            }
        }
    }

    float cut_off_;
    std::vector<float> filtered_;
};

MixedLowPass::MixedLowPass(float cut_off) :
    pimpl_(std::make_unique<Pimpl>(cut_off)) {

}

MixedLowPass::~MixedLowPass() = default;

void MixedLowPass::process(const std::vector<std::vector<float>> &input, std::vector<float> &output) {
    pimpl_->process(input, output);

}

void MixedLowPass::setCutOffFrequency(float cut_off) {
    pimpl_->cut_off_ = cut_off;
}

float MixedLowPass::cutOffFrequency() const {
    return pimpl_->cut_off_;
}

