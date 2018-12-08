#include "../include/resample.hpp"
#include <samplerate.h>
#include "../include/resample.hpp"


using namespace score;

struct ReSampler::Pimpl {

    Pimpl(int channels, Quality quality, float ratio) :
            ratio_(ratio) {

        if (!src_is_valid_ratio(ratio)) {
            throw std::invalid_argument("ratio must be positive real numbers.");
        }

        quality_ = static_cast<std::underlying_type<Quality >::type>(quality);
        state_                   = src_new(quality_, channels, &error_);
        ensure_no_error();
    }

    ~Pimpl() {
        src_delete(state_);
    }

    long process(const float* input, float* output, int size) {
        const auto channels = src_get_channels(state_);
        data_.input_frames      = size / channels;
        data_.output_frames     = size / channels;
        data_.data_in           = input;
        data_.data_out          = output;
        data_.src_ratio         = ratio_;
        data_.input_frames_used = 0;
        data_.output_frames_gen = 0;
        data_.end_of_input      = 0;
        error_                  = src_process(state_, &data_);
        ensure_no_error();
        return data_.output_frames_gen * channels;
    }

    void reset() {
        src_reset(state_);
    }

    float ratio() const {
        return ratio_;
    }

    void setRatio(float ratio) {
        ratio_ = ratio;
        src_set_ratio(state_, ratio);
        ensure_no_error();
    }

    void ensure_no_error() {
        if (error_ != 0) {
            throw std::runtime_error("Error while running re-sampling:" + std::string(src_strerror(error_)));
        }
    }

    int quality() const {
        return quality_;
    }

private:
    SRC_DATA data_{};
    SRC_STATE* state_{nullptr};
    int error_{0};
    int quality_{SRC_SINC_MEDIUM_QUALITY};
    float ratio_{1};
};

ReSampler::ReSampler(std::size_t channels, Quality quality, float ratio) :
    pimpl_(std::make_unique<Pimpl>(channels, quality, ratio)) {

}

ReSampler::~ReSampler() = default;

void ReSampler::reset() {
    pimpl_->reset();
}

std::size_t ReSampler::process(const float* input, float* output, std::size_t size) {
    return static_cast<size_t>(pimpl_->process(input, output, static_cast<int>(size)));
}

std::size_t ReSampler::process(const std::vector<float>& input, std::vector<float>& output) {
    output.resize(input.size());
    const auto used = pimpl_->process(input.data(), output.data(), static_cast<int>(input.size()));
    output.resize((unsigned long) used);
    return (size_t) used;
}

float ReSampler::ratio() const {
    return pimpl_->ratio();
}

void score::ReSampler::setRatio(float ratio) {
    pimpl_->setRatio(ratio);
}

bool score::ReSampler::isValidRatio(float ratio) {
    return static_cast<bool>(src_is_valid_ratio(ratio));
}

ReSampler::Quality score::ReSampler::quality() const {
    return static_cast<Quality>(pimpl_->quality());
}
