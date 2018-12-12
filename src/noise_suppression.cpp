#include <memory>

#include "noise_suppression.hpp"
#include "band_extractor.hpp"
#include "utils.hpp"

#include <noise_suppression.h>

using namespace score;

struct Handler {
    explicit Handler(std::int32_t sample_rate) : handle_(WebRtcNs_Create()) {
        if (handle_ == nullptr) {
            throw std::bad_alloc();
        } else {
            const auto error = WebRtcNs_Init(handle_, static_cast<uint32_t>(sample_rate));
            if (error != 0) {
                throw std::runtime_error("Error while initializing the noise suppression block.");
            }
        }
    }

    ~Handler() {
        WebRtcNs_Free(handle_);
    }

    NsHandle *core() { return handle_; }

private:
    NsHandle *handle_{nullptr};
};



struct NoiseSuppression::Pimpl {

    static constexpr auto ExpectedDuration = 10;
    static constexpr auto MaximumAllowedSize = 160;

    Pimpl(std::int32_t sample_rate, std::int8_t channels, Policy policy) :
        channels_(channels),
        sample_rate_(sample_rate),
        estimated_noise_(WebRtcNs_num_freq(), 0),
        handlers_(channels)
    {
        for (auto& smart_pointer : handlers_) {
            smart_pointer = std::make_unique<Handler>(sample_rate);
        }
        setPolicy(policy);
        resize();
    }

    ~Pimpl() = default;

    void resize() {
        const auto expected_frames = static_cast<std::size_t>(ExpectedDuration * sample_rate_);
        input_data_.resize(expected_frames);
        output_data_.resize(expected_frames);
        input_bands_ptr_[0] = input_data_.data();
        output_bands_ptr_[0] = output_data_.data();
    }
    

    void setPolicy(NoiseSuppression::Policy policy) {
        const auto p = static_cast<std::underlying_type<NoiseSuppression::Policy>::type>(policy);
        for (const auto& handler : handlers_) {
            const auto error = WebRtcNs_set_policy(handler->core(), p);
            if (error != 0) {
                throw std::runtime_error("Error while updating noise suppression policy.");
            }
        }
        policy_ = policy;
    }

    std::size_t numberFrequencyBins() const {
        return WebRtcNs_num_freq();
    }

    const std::vector<float>& estimatedNoise() {
        float fraction = 1.0f / channels_;
        for (auto i = 0; i < channels_; ++i) {
            const auto* noise_bands  = WebRtcNs_noise_estimate(handlers_[i]->core());
            for (auto j = 0ul, size = estimated_noise_.size(); j < size; ++j) {
                estimated_noise_[j] += fraction * noise_bands[j];
            }
        }
        return estimated_noise_;
    }

    void reset() {

    }

    float speechProbability() {
        auto probability = 0.0f;
        for (auto i = 0; i < channels_; ++i) {
            probability += WebRtcNs_prior_speech_probability(handlers_[i]->core());
        }

        if (channels_) {
            probability /= static_cast<float>(channels_);
        }

        return probability;
    }

    void process(const AudioBuffer &input,
                 AudioBuffer &output) {
        if (input.channels() != channels_) {
            throw std::invalid_argument("Expected an input frame with " + std::to_string(channels_) + " channels.");
        }

        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Expected an input frame at " + std::to_string(sample_rate_) + " Hz.");
        }

        const auto duration = input.duration();
        if (input.duration() != ExpectedDuration) {
            throw std::invalid_argument("Expected a frame of 10 msecs");
        }

        if (input.framesPerChannel() > MaximumAllowedSize)  {
            throw std::invalid_argument("Expected a maximum of 160 samples per band");
        }

        output.setSampleRate(sample_rate_);
        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < channels_; ++i) {
            Converter::S16ToFloatS16(input.channel(i), input.framesPerChannel(), input_bands_ptr_[0]);
            {
                WebRtcNs_Analyze(handlers_[i]->core(), input_data_.data());
                WebRtcNs_Process(handlers_[i]->core(), &input_bands_ptr_[0], 1, &output_bands_ptr_[0]);
            }
            Converter::FloatS16ToS16(input_bands_ptr_[0], output.framesPerChannel(), output.channel(i));
        }
    }



private:
    Policy policy_;
    std::int32_t sample_rate_{};
    std::int8_t channels_{};
    std::vector<float> estimated_noise_;
    std::vector<std::unique_ptr<Handler>> handlers_{};
    std::vector<float> input_data_;
    std::vector<float> output_data_;
    std::array<float*, 1> input_bands_ptr_;
    std::array<float*, 1> output_bands_ptr_;
    BandExtractor band_extractor_{};
    
};

NoiseSuppression::NoiseSuppression(std::int32_t sample_rate, std::int8_t channels, Policy policy)
: pimpl_(std::make_unique<Pimpl>(sample_rate, channels, policy)) {}

NoiseSuppression::~NoiseSuppression() = default;


void NoiseSuppression::reset() {
    pimpl_->reset();
}

void NoiseSuppression::process(const AudioBuffer &input,
                                      AudioBuffer &output) {
    pimpl_->process(input, output);

}

float NoiseSuppression::speechProbability() const {
    return pimpl_->speechProbability();
}

std::size_t score::NoiseSuppression::numberFrequencyBins() const {
    return pimpl_->numberFrequencyBins();
}

const std::vector<float> &score::NoiseSuppression::estimatedNoise() const {
    return pimpl_->estimatedNoise();
}

void NoiseSuppression::setPolicy(NoiseSuppression::Policy policy) {
    pimpl_->setPolicy(policy);
}
