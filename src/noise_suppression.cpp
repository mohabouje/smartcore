#include <memory>

#include "noise_suppression.hpp"
#include "band_extractor.hpp"
#include <noise_suppression.h>



using namespace score;

struct Handler {
    explicit Handler(float sample_rate) : handle_(WebRtcNs_Create()) {
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


    Pimpl(std::size_t channels, float sample_rate, Policy policy) :
        channels_(channels),
        sample_rate_(sample_rate),
        estimated_noise_(WebRtcNs_num_freq(), 0),
        handlers_(channels),
        expected_frames_(0.01 * sample_rate_)
    {
        for (auto& smart_pointer : handlers_) {
            smart_pointer = std::make_unique<Handler>(sample_rate);
        }
        setPolicy(policy_);
    }

    ~Pimpl() = default;

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
            probability  += WebRtcNs_prior_speech_probability(handlers_[i]->core());
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

        if (input.framesPerChannel() != expected_frames_) {
            throw std::invalid_argument("Expected an input frame with "
            + std::to_string(expected_frames_) + " samples per buffer");
        }

        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < channels_; ++i) {

            input_bands_[Band0To8kHz] = input.band_f(i, Band0To8kHz);
            input_bands_[Band8To16kHz] = input.band_f(i, Band8To16kHz);

            output_bands_[Band0To8kHz] = output.band_f(i, Band0To8kHz);
            output_bands_[Band8To16kHz] = output.band_f(i, Band8To16kHz);

            WebRtcNs_Analyze(handlers_[i]->core(), input_bands_[Band0To8kHz]);
            WebRtcNs_Process(handlers_[i]->core(), &input_bands_.front(),
                    Bands::NumberBands, &output_bands_.front());
        }
        output.merge();
    }



private:
    Policy policy_;
    float sample_rate_{};
    std::size_t channels_{};
    std::size_t expected_frames_{};
    std::vector<float> estimated_noise_;
    std::array<const float*, Bands::NumberBands> input_bands_;
    std::array<float*, Bands::NumberBands> output_bands_;
    std::vector<std::unique_ptr<Handler>> handlers_{};
};

NoiseSuppression::NoiseSuppression(std::size_t channels, float sample_rate, Policy policy)
: pimpl_(std::make_unique<Pimpl>(channels, sample_rate, policy)) {}

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
