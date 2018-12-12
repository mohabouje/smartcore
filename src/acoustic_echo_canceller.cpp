#include "acoustic_echo_canceller.hpp"
#include <speex/speex_echo.h>

using namespace score;

struct AEC::Pimpl {

    Pimpl(std::int32_t sample_rate,
          std::size_t record_channels,
          std::size_t played_channels,
          std::size_t frame_size,
          std::size_t filter_length) :
        record_channels_(record_channels),
        play_channels_(played_channels)
    {

        if (frame_size > 0.02 * sample_rate) {
            throw std::invalid_argument("Number of samples to process at one time "
                                        "(should correspond to 10-20 ms)");
        }

        if (filter_length > 0.5 * sample_rate) {
            throw std::invalid_argument("Number of samples of echo to cancel "
                                        "(should generally correspond to 100-500 ms)");
        }

        state_ = speex_echo_state_init_mc(static_cast<int>(frame_size),
                                          static_cast<int>(filter_length),
                                          static_cast<int>(record_channels),
                                          static_cast<int>(played_channels));
        if (state_ == nullptr) {
            throw std::bad_alloc();
        }

        speex_echo_ctl(state_, SPEEX_ECHO_SET_SAMPLING_RATE, &sample_rate);
    }

    ~Pimpl() {
        speex_echo_state_destroy(state_);
    }

    void reset() {
        speex_echo_state_reset(state_);
    }
    
    void process(const AudioBuffer& recorded, const AudioBuffer& played, AudioBuffer& output) {
        int sample_rate = 0;
        speex_echo_ctl(state_, SPEEX_ECHO_GET_SAMPLING_RATE, &sample_rate);

        if (recorded.sampleRate() != played.sampleRate() || recorded.sampleRate() != sample_rate) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected " + std::to_string(sample_rate) + " Hz");
        }
        
        if (record_channels_ != recorded.channels()) {
            throw std::invalid_argument("The AEC is configure to work with " 
                                        + std::to_string(record_channels_) + " record channels.");
        }

        if (play_channels_ != played.channels()) {
            throw std::invalid_argument("The AEC is configure to work with "
                                        + std::to_string(play_channels_) + " play channels.");
        }

        int frame_size = 0;
        speex_echo_ctl(state_, SPEEX_ECHO_GET_FRAME_SIZE, &frame_size);
        if (recorded.framesPerChannel() != frame_size || played.framesPerChannel() != frame_size) {
            throw std::invalid_argument("The AEC is configure to work with " + std::to_string(frame_size)
            + " frames per buffer.");
        }

        output.setSampleRate(sample_rate);
        output.resize(recorded.channels(), recorded.framesPerChannel());
        speex_echo_cancellation(state_, recorded.interleave(), played.interleave(), output.interleave());
    }

private:
    SpeexEchoState* state_{nullptr};
    std::size_t record_channels_;
    std::size_t play_channels_;
};

score::AEC::AEC(std::int32_t sample_rate, std::size_t record_channels, std::size_t played_channels, std::size_t frame_size,
                std::size_t filter_length) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, record_channels, played_channels, frame_size, filter_length)) {

}

void score::AEC::process(const AudioBuffer &recorded, const AudioBuffer &played, AudioBuffer &output) {
    pimpl_->process(recorded, played, output);
}

score::AEC::~AEC() = default;
