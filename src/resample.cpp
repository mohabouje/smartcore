#include "../include/resample.hpp"
#include <samplerate.h>
#include "../include/resample.hpp"


using namespace score;



struct ReSampler::Pimpl {

    struct Handler {

        Handler(Quality quality, float ratio)  {

            if (!src_is_valid_ratio(ratio)) {
                throw std::invalid_argument("ratio must be positive real numbers.");
            }

            int error = 0;
            state_  = src_new(static_cast<std::underlying_type<Quality >::type>(quality), 1, &error);
            if (error != 0) {
                throw std::runtime_error("Error while running re-sampling:" + std::string(src_strerror(error)));
            }

            setRatio(ratio);
        }

        ~Handler() {
            src_delete(state_);
        }

        void reset() {
            src_reset(state_);
        }

        void setRatio(float ratio) {
            if (auto error = src_set_ratio(state_, ratio) != 0) {
                throw std::runtime_error("Error while running re-sampling:" + std::string(src_strerror(error)));
            }
        }

        std::size_t process(const float* input, std::size_t size, float ratio, float* output) {
            data_.input_frames      = size;
            data_.output_frames     = size ;
            data_.data_in           = const_cast<float*>(input);
            data_.data_out          = output;
            data_.src_ratio         = ratio;
            data_.input_frames_used = 0;
            data_.output_frames_gen = 0;
            data_.end_of_input      = 0;

            if (auto error = src_process(state_, &data_) != 0) {
                throw std::runtime_error("Error while running re-sampling:" + std::string(src_strerror(error)));
            }

            return static_cast<size_t>(data_.output_frames_gen);
        }


        SRC_DATA data_{};
        SRC_STATE* state_{nullptr};
    };


    Pimpl(std::size_t channels, Quality quality, float ratio) :
            channels_(channels),
            ratio_(ratio),
            handler_(channels, Handler(quality, ratio)) {

    }

    ~Pimpl() = default;

    void process(const AudioBuffer& input, AudioBuffer& output) {
        if (input.channels() != channels_) {
            throw std::invalid_argument("Expected an input frame with " + std::to_string(channels_) + " channels.");
        }

        if (output.sampleRate() != input.sampleRate() * ratio_) {
            throw std::runtime_error("Expecting an output buffer of "
                                     + std::to_string(input.sampleRate() * ratio_) + " Hz.");
        }

        const auto expected_size = static_cast<std::size_t >(input.framesPerChannel() * ratio_);
        output.resize(input.channels(), expected_size);

        auto minimum_size = expected_size;
        for (auto i = 0ul; i < channels_; ++i) {
            const auto& in = input.channel_f(i);
            auto& out = output.channel_f(i);
            minimum_size = std::min(handler_[i].process(in.data(), in.size(), ratio_, out.data()), minimum_size);
        }
        output.resize(input.channels(), minimum_size);
    }

    void reset() {
        for (auto& h : handler_) {
            h.reset();
        }
    }

    float ratio() const {
        return ratio_;
    }

    int quality() const {
        return quality_;
    }

    void setRatio(float ratio) {
        for (auto& h : handler_) {
            h.setRatio(ratio);
        }
        ratio_ = ratio;
    }

    std::vector<Handler> handler_;
    int quality_{SRC_SINC_MEDIUM_QUALITY};
    std::size_t channels_;
    float ratio_{1};
};

ReSampler::ReSampler(std::size_t channels, Quality quality, float ratio) :
    pimpl_(std::make_unique<Pimpl>(channels, quality, ratio)) {

}

ReSampler::~ReSampler() = default;

void ReSampler::reset() {
    pimpl_->reset();
}


void ReSampler::process(const AudioBuffer& input, AudioBuffer& output) {
    pimpl_->process(input, output);
}

float ReSampler::ratio() const {
    return pimpl_->ratio_;
}

void score::ReSampler::setRatio(float ratio) {
    pimpl_->setRatio(ratio);
}

bool score::ReSampler::isValidRatio(float ratio) {
    return static_cast<bool>(src_is_valid_ratio(ratio));
}

ReSampler::Quality score::ReSampler::quality() const {
    return static_cast<Quality>(pimpl_->quality());
}
