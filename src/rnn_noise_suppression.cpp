#include "rnn_noise_suppression.hpp"
#include "utils.hpp"

#include <rnnoise.h>

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

    static constexpr std::int32_t DefaultSampleRate = 48000;
    static constexpr std::size_t DefaultBufferSize = 480;

    explicit Pimpl(std::int8_t channels) :
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

        if (&input == &output) {
            throw std::invalid_argument("The input buffer and output buffer could not be the same");
        }

        output.setSampleRate(DefaultSampleRate);
        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < channels_; ++i) {
            Converter::S16ToFloat(input.channel(i), DefaultBufferSize, input_.data());
            {
                rnnoise_process_frame(handlers_[i]->core(), output_.data(), input_.data());
            }
            Converter::FloatToS16(output_.data(), DefaultBufferSize, output.channel(i));
        }

    }

private:
    std::int8_t channels_;
    std::vector<std::unique_ptr<Handler>> handlers_;
    std::array<float, DefaultBufferSize> input_;
    std::array<float, DefaultBufferSize> output_;
};



DeepNoiseSuppression::DeepNoiseSuppression(std::int8_t channels) : pimpl_(std::make_unique<Pimpl>(channels)){}

DeepNoiseSuppression::~DeepNoiseSuppression() = default;

void DeepNoiseSuppression::process(const AudioBuffer& input, AudioBuffer& output) {
    pimpl_->process(input, output);
}

void DeepNoiseSuppression::reset() {
    pimpl_->reset();
}
