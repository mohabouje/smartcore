#include "../include/vad.hpp"
#include <fvad.h>
#include <array>
#include <algorithm>

using namespace score;

struct VAD::Pimpl {
    explicit Pimpl(std::int32_t sampleRate) :
        sample_rate_(sampleRate),
        processor_(fvad_new()) {

        if (processor_ == nullptr) {
            throw std::bad_alloc();
        }

        setSampleRate(sampleRate);
        setMode(mode_);
    }

    ~Pimpl() {
        fvad_free(processor_);
    }

    void setSampleRate(std::int32_t sampleRate) {
        const auto result = fvad_set_sample_rate(processor_, sampleRate);
        if (result == -1) {
            throw std::invalid_argument("Invalid sample rate. "
                                        "Valid values are 8000, 16000, 32000 and 48000Hz.");
        }
        sample_rate_ = sampleRate;
    }

    int sampleRate() const {
        return static_cast<int>(sample_rate_);
    }

    bool process(const std::int16_t *samples, std::size_t numberSamples) {
        const auto result = fvad_process(processor_, samples, numberSamples);
        if (result == -1) {
            throw std::invalid_argument("Invalid frame length. Must be either 10, 20 or 30 msecs.");
        }
        return (bool) result;
    }

    void reset() {
        fvad_reset(processor_);
    }

    void setMode(VAD::Mode mode) {
        const auto m = static_cast<std::underlying_type<VAD::Mode>::type>(mode);
        fvad_set_mode(processor_, m);
    }

    Fvad* processor_{nullptr};
    VAD::Mode mode_{VAD::Mode::Quality};
    float sample_rate_;
};

VAD::VAD(std::int32_t sampleRate) : pimpl_(std::make_unique<Pimpl>(sampleRate))  {

}

std::vector<std::int32_t> VAD::supportedSampleRates() {
    return {8000, 16000, 32000, 48000};
}

std::vector<std::int32_t> VAD::supportedFrameLength() {
    return {10, 20, 30 };
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

bool VAD::process(const AudioBuffer &samples) {
    return pimpl_->process(samples.downmix().data(), samples.framesPerChannel());
}

VAD::~VAD() = default;
