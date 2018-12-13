
#include <rnn_vad.hpp>
#include <rnn_vad/rnn.h>
#include <rnn_vad/features_extraction.h>

struct score::DeepVAD::Pimpl {

    bool process(const AudioBuffer& input) {
        constexpr auto DefaultSampleRate = 24000;
        constexpr auto DefaultBufferSize = 240;

        if (input.channels() != 1) {
            throw std::runtime_error("Expected a mono (single channel) input frame.");
        }

        if (input.sampleRate() != webrtc::rnn_vad::kSampleRate24kHz) {
            throw std::invalid_argument("Invalid sample rate. Supported sample rate: "
                                        + std::to_string(webrtc::rnn_vad::kSampleRate24kHz) + " Hz.");
        }

        if (input.framesPerChannel() != webrtc::rnn_vad::kFrameSize10ms24kHz) {
            throw std::invalid_argument("Invalid frame length. Expected a buffer of : "
                                        + std::to_string(webrtc::rnn_vad::kFrameSize10ms24kHz) + " samples.");
        }

        is_silence_ = features_extractor_.CheckSilenceComputeFeatures({input.data(), input.size()}, feature_vector_);
        vad_probability_ = vad_.ComputeVadProbability(feature_vector_, is_silence_);
        return is_silence_;
    }

    webrtc::rnn_vad::RnnBasedVad vad_{};
    bool is_silence_{true};
    float vad_probability_{0};
    std::array<float, webrtc::rnn_vad::kFeatureVectorSize> feature_vector_{};
    webrtc::rnn_vad::FeaturesExtractor features_extractor_{};
};

score::DeepVAD::DeepVAD() : pimpl_(std::make_unique<Pimpl>()) {

}

score::DeepVAD::~DeepVAD() = default;

void score::DeepVAD::reset() {
    pimpl_->vad_.Reset();
    pimpl_->features_extractor_.Reset();
}

bool score::DeepVAD::process(const score::AudioBuffer &input) {
    return pimpl_->process(input);
}

float score::DeepVAD::speechProbability() const {
    return pimpl_->vad_probability_;
}
