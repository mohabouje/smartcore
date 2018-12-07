#include "noise_suppression.hpp"

#include <rnnoise.h>
#include <noise_suppression.hpp>


using namespace score;

struct NoiseSuppression::Pimpl {
    Pimpl() : processor_(rnnoise_create()) {
        if (processor_ == nullptr) {
            throw std::bad_alloc();
        } else {
            rnnoise_init(processor_);
        }
    }

    ~Pimpl() {
        rnnoise_destroy(processor_);
    }

    void reset() {
        rnnoise_init(processor_);
    }

    void process(const float* input, float* output) {
        rnnoise_process_frame(processor_, output, input);
    }

private:
    DenoiseState* processor_{nullptr};
};

NoiseSuppression::NoiseSuppression() : pimpl_(std::make_unique<Pimpl>()){}

NoiseSuppression::~NoiseSuppression() = default;

void NoiseSuppression::process(const float *input, float *output) {
    pimpl_->process(input, output);
}

void NoiseSuppression::process(const std::vector<float> &input, std::vector<float> &output) {
    static constexpr auto BufferSize = 480;
    if (input.size() != BufferSize) {
        throw std::invalid_argument("Invalid length. Expected 480 samples and 48KHz as sample rate.");
    }

    output.resize(input.size());
    pimpl_->process(input.data(), output.data());
}

void NoiseSuppression::reset() {
    pimpl_->reset();
}
