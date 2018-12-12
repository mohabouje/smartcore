#include "../include/vad.hpp"
#include <fvad.h>
#include <array>
#include <algorithm>

using namespace score;

struct VAD::Pimpl {
    explicit Pimpl(std::int32_t sampleRate, Mode mode) :
        sample_rate_(sampleRate),
        processor_(fvad_new()),
        mode_(mode)
    {

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


    bool process(const AudioBuffer &input) {

        if (input.channels() != 1) {
            throw std::runtime_error("Expected a mono (single channel) input frame.");
        }


        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Invalid sample rate. Supported sample rate: "
                                        + std::to_string(sample_rate_) + " Hz.");
        }

        static auto valid_frame_lengths = SupportedFrameDuration();
        if (std::find(valid_frame_lengths.begin(), valid_frame_lengths.end(), input.duration())
            == valid_frame_lengths.end()) {
            throw std::invalid_argument("Invalid frame legnth. See SupportedFrameDuration for details.");
        }

        const auto result = fvad_process(processor_, input.channel(0), input.framesPerChannel());
        if (result == -1) {
            throw std::runtime_error("Unexpected error");
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
    std::int32_t sample_rate_;
};

VAD::VAD(std::int32_t sampleRate, Mode mode) : pimpl_(std::make_unique<Pimpl>(sampleRate, mode))  {

}

std::vector<std::int32_t> VAD::SupportedSampleRates() {
    return {8000, 16000, 32000, 48000};
}

std::vector<std::int32_t> VAD::SupportedFrameDuration() {
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

bool VAD::process(const AudioBuffer &input) {
    return pimpl_->process(input);
}

VAD::~VAD() = default;
