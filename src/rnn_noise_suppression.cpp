#include "rnn_noise_suppression.hpp"

#include <rnnoise.h>
#include "rnn_noise_suppression.hpp"


using namespace score;

struct Handler {

    Handler() : processor_(rnnoise_create()) {
        if (processor_ == nullptr) {
            throw std::bad_alloc();
        } else {
            rnnoise_init(processor_);
        }
    }

    ~Handler() {
        rnnoise_destroy(processor_);
    }

    DenoiseState* core() const { return processor_; }
private:
    DenoiseState* processor_{nullptr};
};


struct DeepNoiseSuppression::Pimpl {
    explicit Pimpl(std::size_t channels) :
        channels_(channels),
        handlers_(channels) {
        for (auto& smart_pointer : handlers_) {
            smart_pointer = std::make_unique<Handler>();
        }
    }

    ~Pimpl() = default;

    void reset() {
        for (auto& handler : handlers_) {
            rnnoise_init(handler->core());
        }
    }

    void process(const AudioBuffer& input, AudioBuffer& output) {
        if (input.channels() != channels_) {
            throw std::invalid_argument("Expected an input frame with " + std::to_string(channels_) + " channels.");
        }

        if (input.sampleRate() != DefaultSampleRate) {
            throw std::invalid_argument("Invalid sample rate. Supported sample rate: "
            + std::to_string(DefaultSampleRate) + " Hz.");
        }

        if (input.framesPerChannel() != DefaultBufferSize) {
            throw std::invalid_argument("Invalid length. Expected 480 samples and 48KHz as sample rate.");
        }

        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < channels_; ++i) {
            rnnoise_process_frame(handlers_[i]->core(), output.channel_f(i), input.channel_f(i));
        }

    }

private:
    std::size_t channels_;
    std::vector<std::unique_ptr<Handler>> handlers_;
};

const float DeepNoiseSuppression::DefaultSampleRate = 48000.0f;
const std::size_t DeepNoiseSuppression::DefaultBufferSize = 480;

DeepNoiseSuppression::DeepNoiseSuppression(std::size_t channels) : pimpl_(std::make_unique<Pimpl>(channels)){}

DeepNoiseSuppression::~DeepNoiseSuppression() = default;

void DeepNoiseSuppression::process(const AudioBuffer& input, AudioBuffer& output) {
    pimpl_->process(input, output);
}

void DeepNoiseSuppression::reset() {
    pimpl_->reset();
}
