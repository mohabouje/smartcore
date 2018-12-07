#include "resample.hpp"
#include <libresample.h>

using namespace score;

struct ReSampler::Pimpl {

    Pimpl(Quality quality, float factor) :
        quality_(quality),
        factor_(factor),
        handle_(resample_open(quality_, factor, factor)) {

        if (factor <= 0.0) {
            throw std::invalid_argument("Factor must be positive real numbers.");
        }

        if (handle_ == nullptr) {
            throw std::bad_alloc();
        }
    }

    ~Pimpl() {
        resample_close(handle_);
    }

    int process(const float* input, float* output, int size) {
        int s_used = 0;
        return resample_process(handle_, factor_, input, size, 0, &s_used, output, size);
    }

    void reset() {

    }

private:
    Quality quality_{Quality::Low};
    float factor_{0.0};
    void* handle_{nullptr};
};

ReSampler::ReSampler(Quality quality, float factor) :
    pimpl_(std::make_unique<Pimpl>(quality, factor)) {

}

ReSampler::~ReSampler() = default;

void ReSampler::reset() {
    pimpl_->reset();
}

int ReSampler::process(const float* input, float* output, std::size_t size) {
    return pimpl_->process(input, output, static_cast<int>(size));
}

int ReSampler::process(const std::vector<float>& input, std::vector<float>& output) {
    output.resize(input.size());
    const auto used = pimpl_->process(input.data(), output.data(), static_cast<int>(input.size()));
    output.resize((unsigned long) used);
    return used;
}
