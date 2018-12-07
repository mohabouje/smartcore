#include "vad.hpp"
#include <fvad.h>
#include <array>
#include <algorithm>

using namespace score;

struct VAD::Pimpl {
    explicit Pimpl(std::int32_t sampleRate) :
        processor_(fvad_new()) {

        if (processor_ == nullptr) {
            throw std::bad_alloc();
        }

        setSampleRate(sampleRate);
    }

    ~Pimpl() {
        fvad_free(processor_);
    }

    void setSampleRate(std::int32_t sampleRate) {
        const auto result = fvad_set_sample_rate(processor_, sampleRate);
        if (result == -1) {
            throw std::invalid_argument("Invalid sample rate. "
                                        "Valid values are 8000, 16000, 32000 and 48000.");
        }
    }

    int sampleRate() const {
        return fvad_get_sample_rate(processor_);
    }

    bool process(const std::int16_t *samples, std::size_t numberSamples) {
        const auto result = fvad_process(processor_, samples, numberSamples);
        if (result == -1) {
            throw std::invalid_argument("Invalid frame length. Must be either 80, 160 or 240.");
        }
        return result;
    }

    bool process(const float *samples, std::size_t numberSamples) {
        std::transform(samples, samples + numberSamples, &temp_[0], [](const auto value) {
            return value > 0 ? std::numeric_limits<std::int16_t>::max() * value
                             : std::numeric_limits<std::int16_t>::min() * value;
        });

        const auto result = fvad_process(processor_, temp_.data(), numberSamples);
        if (result == -1) {
            throw std::invalid_argument("Invalid frame length. Must be either 80, 160 or 240.");
        }
        return result;
    }

    void reset() {
        fvad_reset(processor_);
    }

    Fvad* processor_{nullptr};
    VAD::Mode mode_{VAD::Mode::Quality};
    std::array<std::int16_t, 1440> temp_;
};

VAD::VAD(std::int32_t sampleRate) : pimpl_(std::make_unique<Pimpl>(sampleRate))  {

}

std::vector<std::int32_t> VAD::supportedSampleRates() {
    return {8000, 16000, 32000, 48000};
}

std::vector<std::int32_t> VAD::supportedFrameLength() {
    return {80, 160, 240};
}

void VAD::setSampleRate(const std::int32_t sampleRate)  {
    pimpl_->setSampleRate(sampleRate);
}

std::int32_t VAD::sampleRate() const {
    return pimpl_->sampleRate();
}

void VAD::setMode(const VAD::Mode mode) {
    pimpl_->mode_ = mode;
}

VAD::Mode VAD::mode() const {
    return pimpl_->mode_;
}

void VAD::reset() {
    pimpl_->reset();
}

bool VAD::process(const std::vector<std::int16_t> &samples) {
    return pimpl_->process(samples.data(), samples.size());
}

bool VAD::process(const std::int16_t *samples, std::size_t numberSamples) {
    return pimpl_->process(samples, numberSamples);
}

bool VAD::process(const std::vector<float> &samples) {
    return pimpl_->process(samples.data(), samples.size());
}

bool VAD::process(const float *samples, std::size_t numberSamples) {
    return pimpl_->process(samples, numberSamples);
}

VAD::~VAD() = default;
