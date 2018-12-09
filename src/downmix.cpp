
#include <downmix.hpp>

#include "downmix.hpp"

using namespace score;

struct DownMix::Pimpl {

    Pimpl() = default;

    template <typename T>
    void process(const std::vector<std::vector<T>> &inputs, std::vector<T> &output) {
        if (inputs.empty()) {
            return;
        }

        const auto channels = inputs.size();
        const auto samples = inputs.front().size();
        output.resize(samples);

        for (auto i = 0ul; i < channels; ++i) {
            for (auto j = 0ul; j < samples; ++j) {
                output[j] += (inputs[i][j] / static_cast<T>(channels));
            }
        }
    }


};

void DownMix::process(const std::vector<std::vector<std::int16_t>> &input, std::vector<std::int16_t > &output) {
    pimpl_->process(input, output);

}

void DownMix::process(const std::vector<std::vector<float>> &input, std::vector<float> &output) {
    pimpl_->process(input, output);
}
