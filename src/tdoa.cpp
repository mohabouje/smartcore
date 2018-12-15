#include "tdoa.hpp"
#include <tdoa.h>

using namespace score;

struct TDOA::Pimpl {

    Pimpl(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_buffer, std::int8_t reference) :
        sample_rate_(sample_rate),
        channels_(channels),
        reference_(reference),
        indexes_(channels, 0),
        tdoa_(channels, 0),
        frames_per_buffer_(frames_per_buffer),
        margin_(20) {

        if (reference_ > channels_) {
            throw std::runtime_error("Invalid reference microphone");
        }
    }


    const Vector<float>& process(const AudioBuffer& input) {
        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected "
                                        + std::to_string(sample_rate_) + " Hz");
        }

        if (channels_ != input.channels()) {
            throw std::invalid_argument("The TDOA is configured to work with "
                                        + std::to_string(channels_) + " channels.");
        }

        if (frames_per_buffer_ != input.framesPerChannel()) {
            throw std::invalid_argument("The TDOA is configured to work with "
                                        + std::to_string(frames_per_buffer_) + " samples per channel.");
        }


        GccPhatTdoa(input.data(), input.channels(), frames_per_buffer_,
                    reference_, margin_, indexes_.data());

        for (auto i = 0ul; i < channels_; ++i)
            tdoa_[i] = static_cast<float>(indexes_[i]) / static_cast<float>(sample_rate_);

        return tdoa_;
    }

    void setMargin(std::int32_t margin) {
        if (margin > frames_per_buffer_ / 2) {
            throw std::runtime_error("Maximum allowed margin: "
            + std::to_string(frames_per_buffer_ / 2) + "(half of the frames per channels)");
        }
        margin_ = margin;
    }


private:
    std::int32_t margin_;
    std::int32_t sample_rate_;
    std::int8_t channels_;
    std::int8_t reference_;
    std::int32_t frames_per_buffer_;
    std::vector<int> indexes_;
    std::vector<float> tdoa_;

};

score::TDOA::TDOA(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_buffer, int8_t reference) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels, frames_per_buffer, reference)) {

}

score::TDOA::~TDOA() = default;

const Vector<float> score::TDOA::process(const AudioBuffer &input) {
    return pimpl_->process(input);
}

void score::TDOA::setMargin(std::int32_t margin) {
    pimpl_->setMargin(margin);
}
