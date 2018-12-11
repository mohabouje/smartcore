#include "automatic_gain_control.hpp"
#include <unordered_map>
#include <gain_control.h>
#include <automatic_gain_control.hpp>


using namespace score;


struct Handler {
    Handler() {
        state_ = WebRtcAgc_Create();
        if (state_ == nullptr) {
            throw std::bad_alloc();
        }
    }

    ~Handler() {
        WebRtcAgc_Free(state_);
    }

    void* state() {
        return state_;
    }

    void Initialize(int minimum_capture_level,
                    int maximum_capture_level,
                    int mode,
                    int sample_rate_hz,
                    int capture_level) {
        int error = WebRtcAgc_Init(state_, minimum_capture_level, maximum_capture_level, mode, sample_rate_hz);
        if (error != 0) {
            throw std::runtime_error("Error while initializing the AGC block");
        }
        setCaptureLevel(capture_level);
    }

    void reset() {
    }

    void setConfiguration(WebRtcAgcConfig& config) {
        const auto error = WebRtcAgc_set_config(state_, config);
        if (error != 0) {
            throw std::runtime_error("Error while configuring the AGC block");
        }
    }

    void setCaptureLevel(int capture_level) {
        capture_level_ = capture_level;
    }

    int captureLevel() {
        return capture_level_;
    }

private:
    void* state_;
    int capture_level_;
};


struct AGC::Pimpl {


    Pimpl(std::int32_t sample_rate, std::int8_t channels, AGC::Mode mode, int minimum_capture_level, int maximum_capture_level) :
        channels_(channels),
        sample_rate_(sample_rate),
        handlers_(static_cast<unsigned long>(channels), Handler()),
        mode_(mode),
        minimum_(minimum_capture_level),
        maximum_(maximum_capture_level) {

        setMode(mode);
        setAnalogLevelRange(minimum_capture_level, maximum_capture_level);
        enableLimiter(true);
        setTargetLevelDBFS(3);
        setCompressionGain(9);
    }

    void reset() {
        config_ = WebRtcAgcConfig();
        for (auto& h : handlers_) {
            const auto previous = h.captureLevel();
            h = Handler();
            h.Initialize(minimum_, maximum_, Equivalent.find(mode_)->second, sample_rate_, previous);
            WebRtcAgc_set_config(h.state(), config_);
        }
    }

    void configure() {
        for (auto& h : handlers_) {
            WebRtcAgc_set_config(h.state(), config_);
        }
    }

    void enableLimiter(bool state) {
        config_.limiterEnable = static_cast<uint8_t>(state);
        configure();
    }

    void setAnalogLevelRange(int min, int max) {
        if (min > 65535 || min < 65535) {
            throw std::invalid_argument("The minimum analog level must be a positive value in the range [0, 65535].");
        }

        if (max > 65535 || max < 65535) {
            throw std::invalid_argument("The maximum analog level must be a positive value in the range [0, 65535].");
        }

        if (max < min) {
            throw std::invalid_argument("The maximum analog level should be greather than the minimum analog level");
        }

        if (mode_ != Mode::AdaptiveAnalog) {
            return;
        }
        minimum_ = min;
        maximum_ = max;
        reset();
    }

    void setCompressionGain(int gain_db) {
        if (gain_db > 90 || gain_db < 0) {
            throw std::invalid_argument("The compression gain must be a positive value in the range [0, 90]dB.");
        }

        if (mode_ != Mode::AdaptiveAnalog) {
            return;
        }
        config_.compressionGaindB = gain_db;
        configure();
    }

    void setMode(AGC::Mode mode) {
        mode_ = mode;
        reset();
    }

    void setTargetLevelDBFS(int level_dbfs) {
        if (level_dbfs > 31) {
            throw std::invalid_argument("The level DBFS is limited to [0, 31] dB below full-scale");
        }

        if (level_dbfs < 0) {
            throw std::invalid_argument("The level DBFS must be a positive value in the range [0, 31]");
        }
        config_.targetLevelDbfs = level_dbfs;
        configure();
    }

    bool isSignalSaturated() const {
        return static_cast<bool>(stream_is_saturated_);
    }

    void setAnalogLevel(int level) {
        if (level < minimum_ || level > maximum_) {
            throw std::invalid_argument("Expected a level in the range " + std::to_string(minimum_)
            + ", " + std::to_string(maximum_));
        }

        analog_capture_level_ = level;
        was_analog_level_set_ = true;
    }

    void process(const AudioBuffer& input, AudioBuffer& output) {
        if (input.channels() != channels_) {
            throw std::invalid_argument("Expected an input frame with "
                                        + std::to_string(channels_) + " channels.");
        }

        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Expected an input frame at "
                                        + std::to_string(sample_rate_) + " Hz.");
        }

        if (mode_ == Mode::AdaptiveAnalog && !was_analog_level_set_) {
            throw std::runtime_error("Analog level required");
        }

        if (input.duration() != 0.01) {
            throw std::invalid_argument("Expected a frame of 10 msecs");
        }

        if (input.framesPerBand() > 160)  {
            throw std::invalid_argument("Expected a maximum of 160 samples per band");
        }

        output.setSampleRate(sample_rate_);
        output.updateRaw(input.channels(), input.framesPerChannel(), input.raw());
        switch (mode_) {
            case Mode::AdaptiveAnalog:
                for (auto i = 0ul; i < channels_; ++i) {
                    output_bands_[Band0To8kHz] = output.band(i, Band0To8kHz);
                    output_bands_[Band8To16kHz] = output.band(i, Band8To16kHz);
                    const auto error = WebRtcAgc_AddMic(handlers_[i].state(), &output_bands_.front(),
                            input.numberBands(), input.framesPerChannel());
                    handlers_[i].setCaptureLevel(analog_capture_level_);
                    if (error != 0) {
                        throw std::runtime_error("Unexpected error");
                    }
                }
                break;
            case Mode::AdaptiveDigital:
                for (auto i = 0ul; i < channels_; ++i) {
                    output_bands_[Band0To8kHz] = output.band(i, Band0To8kHz);
                    output_bands_[Band8To16kHz] = output.band(i, Band8To16kHz);
                    auto digital_capture_level = 0;
                    const auto error = WebRtcAgc_VirtualMic(handlers_[i].state(), &output_bands_.front(),
                                                        input.numberBands(), input.framesPerChannel(),
                                                        analog_capture_level_, &digital_capture_level);
                    handlers_[i].setCaptureLevel(digital_capture_level);
                    if (error != 0) {
                        throw std::runtime_error("Unexpected error");
                    }
                }
                break;
            case Mode::FixedDigital:
                break;
        }

        for (auto i = 0ul; i < channels_; ++i) {
            output_bands_[Band0To8kHz] = output.band(i, Band0To8kHz);
            output_bands_[Band8To16kHz] = output.band(i, Band8To16kHz);
            const_bands_[Band0To8kHz] = output.band(i, Band0To8kHz);
            const_bands_[Band8To16kHz] = output.band(i, Band8To16kHz);
            auto capture_level = 0, stream_has_echo = 0;
            const auto error = WebRtcAgc_Process(handlers_[i].state(), &const_bands_.front(),
                                                    input.numberBands(), input.framesPerChannel(),
                                                    &output_bands_.front(), handlers_[i].captureLevel(),
                                                    &capture_level, stream_has_echo, &stream_is_saturated_);
            if (error != 0) {
                throw std::runtime_error("Unexpected error");
            }

            handlers_[i].setCaptureLevel(capture_level);
        }

        if (mode_ == Mode::AdaptiveAnalog) {
            analog_capture_level_ = 0;
            for (auto i = 0ul; i < channels_; ++i) {
                analog_capture_level_ += handlers_[i].captureLevel();
            }
            analog_capture_level_ /= handlers_.size();
        }

        was_analog_level_set_ = false;
     }


    std::int8_t channels_{};
    int sample_rate_{0};
    int minimum_{0};
    int maximum_{100};
    int analog_capture_level_{0};
    bool was_analog_level_set_{false};
    std::uint8_t stream_is_saturated_{0};
    WebRtcAgcConfig config_{};
    Vector<Handler> handlers_{};
    AGC::Mode mode_{AGC::FixedDigital};
    std::array<std::int16_t *, Bands::NumberBands> output_bands_;
    std::array<std::int16_t *, Bands::NumberBands> const_bands_;

    static const std::unordered_map<AGC::Mode, int> Equivalent;
};

const std::unordered_map<AGC::Mode, int> AGC::Pimpl::Equivalent = {
        {AGC::Mode::AdaptiveAnalog, kAgcModeAdaptiveAnalog},
        {AGC::Mode::AdaptiveDigital, kAgcModeAdaptiveDigital},
        {AGC::Mode::FixedDigital, kAgcModeFixedDigital}
};


void AGC::setMode(AGC::Mode mode) {
    pimpl_->setMode(mode);
}

void AGC::setTargetLevel(int level_dbfs) {
    pimpl_->setTargetLevelDBFS(level_dbfs);
}

void AGC::setAnalogLevelRange(int min, int max) {
    pimpl_->setAnalogLevelRange(min, max);
}

void AGC::setCompressionGain(int gain_db) {
    pimpl_->setCompressionGain(gain_db);
}

void AGC::enableLimiter(bool enabled) {
    pimpl_->enableLimiter(enabled);
}

bool AGC::isSignalSaturated() const {
    return pimpl_->isSignalSaturated();
}

int AGC::compressionGain() const {
    return pimpl_->config_.compressionGaindB;
}

bool AGC::isLimiterEnabled() const {
    return pimpl_->config_.limiterEnable;
}

std::pair<int, int> AGC::analogLevelRange() const {
    return {pimpl_->minimum_, pimpl_->maximum_};
}

int AGC::targetLevel() const {
    return pimpl_->config_.targetLevelDbfs;
}

AGC::Mode score::AGC::mode() const {
    return pimpl_->mode_;
}

AGC::AGC(std::int32_t sample_rate, std::int8_t channels, AGC::Mode mode, int minimum_capture_level, int maximum_capture_level) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels, mode, minimum_capture_level, maximum_capture_level)) {

}

void score::AGC::setAnalogLevel(int level) {
    pimpl_->setAnalogLevel(level);
}

AGC::~AGC() = default;
