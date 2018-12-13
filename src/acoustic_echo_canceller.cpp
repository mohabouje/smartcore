#include "acoustic_echo_canceller.hpp"
#include "utils.hpp"
#include <speex/speex_echo.h>

using namespace score;

struct AEC::Pimpl {

    struct Handler {

        Handler(std::int32_t sample_rate, std::size_t frame_size, std::size_t filter_length) {
            state_ = speex_echo_state_init(static_cast<int>(frame_size),
                                              static_cast<int>(filter_length));
            if (state_ == nullptr) {
                throw std::bad_alloc();
            }

            speex_echo_ctl(state_, SPEEX_ECHO_SET_SAMPLING_RATE, &sample_rate);
        }

        ~Handler() {
            speex_echo_state_destroy(state_);
        }

        SpeexEchoState* state_;
    };

    Pimpl(std::int32_t sample_rate, std::int8_t channels, std::size_t frame_size, std::size_t filter_length) :
        channels_(channels),
        record_(frame_size),
        play_(frame_size),
        clean_(frame_size),
        states_(channels, Handler(sample_rate, frame_size, filter_length))
    {

        if (frame_size > 0.02 * sample_rate) {
            throw std::invalid_argument("Number of samples to process at one time "
                                        "(should correspond to 10-20 ms)");
        }

        if (filter_length > 0.5 * sample_rate) {
            throw std::invalid_argument("Number of samples of echo to cancel "
                                        "(should generally correspond to 100-500 ms)");
        }


    }

    ~Pimpl() {
    }

    void reset() {
        for (auto& state : states_)
            speex_echo_state_reset(state.state_);
    }
    
    void process(const AudioBuffer& recorded, const AudioBuffer& played, AudioBuffer& output) {
        int sample_rate = 0;
        speex_echo_ctl(states_.front().state_, SPEEX_ECHO_GET_SAMPLING_RATE, &sample_rate);

        if (recorded.sampleRate() != played.sampleRate() || recorded.sampleRate() != sample_rate) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected " + std::to_string(sample_rate) + " Hz");
        }
        
        if (channels_ != recorded.channels()) {
            throw std::invalid_argument("The AEC is configure to work with " 
                                        + std::to_string(channels_) + " record channels.");
        }

        if (channels_ != played.channels()) {
            throw std::invalid_argument("The AEC is configure to work with "
                                        + std::to_string(channels_) + " play channels.");
        }

        int frame_size = 0;
        speex_echo_ctl(states_.front().state_, SPEEX_ECHO_GET_FRAME_SIZE, &frame_size);
        if (recorded.framesPerChannel() != frame_size || played.framesPerChannel() != frame_size) {
            throw std::invalid_argument("The AEC is configure to work with " + std::to_string(frame_size)
            + " frames per buffer.");
        }

        output.setSampleRate(sample_rate);
        output.resize(channels_, frame_size);
        for (auto i = 0ul; i < channels_; ++i) {
            Converter::FloatS16ToS16(recorded.channel(i), recorded.framesPerChannel(), record_.data());
            Converter::FloatS16ToS16(played.channel(i), played.framesPerChannel(), play_.data());
            speex_echo_cancellation(states_[i].state_, record_.data(), play_.data(), clean_.data());
            Converter::S16ToFloatS16(clean_.data(), clean_.size(), output.channel(i));
        }
    }

private:
    std::vector<Handler>  states_{};
    std::vector<std::int16_t> record_;
    std::vector<std::int16_t> play_;
    std::vector<std::int16_t> clean_;
    std::int8_t channels_;
};

score::AEC::AEC(std::int32_t sample_rate, std::int8_t channels, std::size_t frame_size, std::size_t filter_length) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels, frame_size, filter_length)) {

}

void score::AEC::process(const AudioBuffer &recorded, const AudioBuffer &played, AudioBuffer &output) {
    pimpl_->process(recorded, played, output);
}

score::AEC::~AEC() = default;
