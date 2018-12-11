#include "automatic_gain_control.hpp"
#include <webrtc_audio_processing/audio_processing.h>
#include <webrtc_audio_processing/module_common_types.h>
#include <unordered_map>

using namespace score;

struct AGC::Pimpl {


    Pimpl(std::int32_t sample_rate, std::int8_t channels, AGC::Mode mode) {
        processor_ = webrtc::AudioProcessing::Create(std::rand());
        if (processor_ == nullptr) {
            throw std::bad_alloc();
        }

        processor_->set_num_channels(static_cast<int>(channels), static_cast<int>(channels));
        processor_->set_sample_rate_hz(static_cast<int>(sample_rate));
        processor_->echo_control_mobile()->Enable(false);
        processor_->echo_cancellation()->Enable(false);
        processor_->voice_detection()->Enable(false);
        processor_->level_estimator()->Enable(false);
        processor_->noise_suppression()->Enable(false);
        processor_->high_pass_filter()->Enable(false);
        processor_->gain_control()->Enable(true);

        setMode(mode);
        setTargetLevelDBFS(30);
        setCompressionGain(0);
        setAnalogLevelRange(0, 100);
        enableLimiter(false);
    }

    ~Pimpl() {
        webrtc::AudioProcessing::Destroy(processor_);
    }

    void enableLimiter(bool state) {
        processor_->gain_control()->enable_limiter(state);
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

        processor_->gain_control()->set_analog_level_limits(min, max);
    }

    void setCompressionGain(int gain_db) {
        if (gain_db > 90 || gain_db < 0) {
            throw std::invalid_argument("The compression gain must be a positive value in the range [0, 90]dB.");
        }

        if (mode_ != Mode::AdaptiveAnalog) {
            return;
        }

        processor_->gain_control()->set_compression_gain_db(gain_db);
    }

    void setMode(AGC::Mode mode) {
        mode_ = mode;
        processor_->gain_control()->set_mode(Equivalent.find(mode)->second);
    }

    void setTargetLevelDBFS(int level_dbfs) {
        if (level_dbfs > 31) {
            throw std::invalid_argument("The level DBFS is limited to [0, 31] dB below full-scale");
        }

        if (level_dbfs < 0) {
            throw std::invalid_argument("The level DBFS must be a positive value in the range [0, 31]");
        }
        processor_->gain_control()->set_target_level_dbfs(-level_dbfs);
    }

    bool isSignalSaturated() const {
        return processor_->gain_control()->stream_is_saturated();
    }


    void process(const AudioBuffer& input, AudioBuffer& output) {
        if (input.channels() != processor_->num_input_channels()) {
            throw std::invalid_argument("Expected an input frame with "
                                        + std::to_string(processor_->num_input_channels()) + " channels.");
        }

        if (input.sampleRate() != processor_->sample_rate_hz()) {
            throw std::invalid_argument("Expected an input frame at "
                                        + std::to_string(processor_->sample_rate_hz()) + " Hz.");
        }

        frame_.UpdateFrame(0,
                          static_cast<const WebRtc_UWord32>(input.timestamp()),
                          input.raw(),
                          static_cast<const WebRtc_UWord16>(input.size()),
                          static_cast<const int>(input.sampleRate()),
                          webrtc::AudioFrame::SpeechType::kNormalSpeech,
                          webrtc::AudioFrame::VADActivity::kVadUnknown,
                          static_cast<const WebRtc_UWord8>(input.framesPerChannel()));
        processor_->ProcessStream(&frame_);
        output.setSampleRate(processor_->sample_rate_hz());
        output.updateRaw(input.channels(), input.framesPerChannel(), frame_._payloadData);
    }

    webrtc::AudioProcessing* processor_{nullptr};
    webrtc::AudioFrame frame_;
    AGC::Mode mode_{AGC::FixedDigital};
    static const std::unordered_map<AGC::Mode, webrtc::GainControl::Mode> Equivalent;
};

const std::unordered_map<AGC::Mode, webrtc::GainControl::Mode> AGC::Pimpl::Equivalent = {
        {AGC::Mode::AdaptiveAnalog, webrtc::GainControl::kAdaptiveAnalog},
        {AGC::Mode::AdaptiveDigital, webrtc::GainControl::kAdaptiveDigital},
        {AGC::Mode::FixedDigital, webrtc::GainControl::kFixedDigital}
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
    return pimpl_->processor_->gain_control()->compression_gain_db();
}

bool AGC::isLimiterEnabled() const {
    return pimpl_->processor_->gain_control()->is_limiter_enabled();
}

std::pair<int, int> AGC::analogLevelRange() const {
    return {pimpl_->processor_->gain_control()->analog_level_minimum(),
            pimpl_->processor_->gain_control()->analog_level_maximum()};
}

int AGC::targetLevel() const {
    return pimpl_->processor_->gain_control()->target_level_dbfs();
}

AGC::Mode score::AGC::mode() const {
    return pimpl_->mode_;
}

AGC::AGC(std::int32_t sample_rate, std::int8_t channels, AGC::Mode mode) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels, mode)) {

}

AGC::~AGC() = default;
