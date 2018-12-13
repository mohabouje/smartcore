#include "dereverberation.hpp"
#include "utils.hpp"
#include <speex/speex_preprocess.h>

using namespace score;

struct Handler {
    Handler(std::int32_t sample_rate,
            std::size_t frame_size) {

        if (frame_size > 0.02 * sample_rate) {
            throw std::invalid_argument("Number of samples to process at one time "
                                        "(should correspond to 10-20 ms)");
        }


        state_ = speex_preprocess_state_init(static_cast<int>(sample_rate), static_cast<int>(frame_size));
        if (state_ == nullptr) {
            throw std::bad_alloc();
        }

        int disable = 0, enable = 1;
        speex_preprocess_ctl(state_, SPEEX_PREPROCESS_SET_AGC, &disable);
        speex_preprocess_ctl(state_, SPEEX_PREPROCESS_SET_DENOISE, &disable);
        speex_preprocess_ctl(state_, SPEEX_PREPROCESS_SET_DEREVERB, &enable);
        speex_preprocess_ctl(state_, SPEEX_PREPROCESS_SET_VAD, &disable);
        speex_preprocess_ctl(state_, SPEEX_PREPROCESS_SET_ECHO_STATE, &disable);
    }

    ~Handler() {
        speex_preprocess_state_destroy(state_);
    }

    void reset() {
    }

    SpeexPreprocessState* state_{nullptr};
};


struct DeReverberation::Pimpl {

    Pimpl(std::int32_t sample_rate,
          std::int8_t channels,
          std::size_t frame_size) :
            sample_rate_(sample_rate),
            channels_(channels),
            temp_(frame_size),
            handlers_(static_cast<unsigned long>(channels), Handler(sample_rate, frame_size))

    {

    }

    void reset() {
        for (auto& h : handlers_) {
            h.reset();
        }

    }


    void process(const AudioBuffer& input, AudioBuffer& output) {

        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected "
                                        + std::to_string(sample_rate_) + " Hz");
        }

        if (channels_ != input.channels()) {
            throw std::invalid_argument("The DeReverberation is configure to work with "
                                        + std::to_string(channels_) + " channels.");
        }

        if (input.framesPerChannel() != temp_.size()) {
            throw std::invalid_argument("The DeReverberation is configure to work with " + std::to_string(temp_.size())
                                        + " frames per buffer.");
        }

        input.copyTo(output);
        for (auto i = 0ul; i < channels_; ++i) {
            Converter::FloatS16ToS16(input.channel(i), input.framesPerChannel(), temp_.data());
            speex_preprocess_run(handlers_[i].state_, temp_.data());
            Converter::S16ToFloatS16(temp_.data(), output.framesPerChannel(), output.channel(i));
        }
    }

    void setLevel(int level_db) {
        for (auto& h : handlers_) {
            speex_preprocess_ctl(h.state_, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &level_db);
        }
    }

    void setDecay(int decay) {
        for (auto& h : handlers_) {
            speex_preprocess_ctl(h.state_, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &decay);
        }
    }

    int level() const {
        int level_db = 0;
        speex_preprocess_ctl(handlers_.front().state_, SPEEX_PREPROCESS_GET_DEREVERB_LEVEL, &level_db);
        return level_db;
    }

    int decay() const {
        int decay = 0;
        speex_preprocess_ctl(handlers_.front().state_, SPEEX_PREPROCESS_GET_DEREVERB_DECAY, &decay);
        return decay;
    }

private:
    std::vector<Handler> handlers_;
    std::vector<std::int16_t> temp_;
    std::int32_t sample_rate_;
    std::int8_t channels_;
};

score::DeReverberation::DeReverberation(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_buffer) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels, frames_per_buffer)) {

}

int score::DeReverberation::level() const {
    return pimpl_->level();
}

void score::DeReverberation::setLevel(int level_db) {
    pimpl_->setLevel(level_db);
}

int score::DeReverberation::decay() const {
    return pimpl_->decay();
}

void score::DeReverberation::setDecay(int decay) {
    pimpl_->setDecay(decay);

}

void score::DeReverberation::process(const AudioBuffer &input, AudioBuffer &output) {
    pimpl_->process(input, output);
}

score::DeReverberation::~DeReverberation() = default;
