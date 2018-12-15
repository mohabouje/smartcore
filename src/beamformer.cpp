
#include <beamformer.hpp>

#include "beamformer.hpp"
#include "downmix.hpp"
#include "tdoa.h"
#include "mvdr.h"
#include "gsc.h"
#include "ds.h"
#include "utils.h"

using namespace score;

uint32_t nextPowerOfTwo(uint32_t n)
{
    --n;

    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;

    return n + 1;
}

struct Beamformer::Pimpl {

    Pimpl(std::int32_t sample_rate,
            std::int8_t channels,
            std::size_t frames_per_channel,
            float learning_rate,
            std::int8_t reference) :
        sample_rate_(sample_rate),
        channels_(channels),
        frames_per_buffer_(frames_per_channel),
        nfft_(nextPowerOfTwo(frames_per_channel)),
        learning_rate_(learning_rate),
        reference_(reference),
        indexes_(channels, 0),
        tdoas_(channels, 0),
        gsc_(channels, frames_per_channel, learning_rate_),
        mvdr_(sample_rate, nfft_,  channels),
        margin_(20)
        {

    }


    void procees(const AudioBuffer& input, AudioBuffer& output) {
        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected "
                                        + std::to_string(sample_rate_) + " Hz");
        }

        if (channels_ != input.channels()) {
            throw std::invalid_argument("The Beamformer is configured to work with "
                                        + std::to_string(channels_) + " channels.");
        }

        if (frames_per_buffer_ != input.framesPerChannel()) {
            throw std::invalid_argument("The Beamformer is configured to work with "
                                        + std::to_string(frames_per_buffer_) + " samples per channel.");
        }

        if (!ignore_toa_) {
            GccPhatTdoa(input.data(), input.channels(), static_cast<int>(input.framesPerChannel()),
                        reference_, margin_, indexes_.data());
            for (auto i = 0ul; i < channels_; ++i)
                tdoas_[i] = static_cast<float>(indexes_[i]) / static_cast<float>(sample_rate_);
        }

        output.setSampleRate(input.sampleRate());
        output.resize(1, input.framesPerChannel());

        switch (method_) {
            case Method::DelayAndSum:
                ::DelayAndSum(input.data(), input.channels(), input.framesPerChannel(),
                        indexes_.data(), output.data());
                break;
            case Method::MVDR:
                mvdr_.DoBeamformimg(input.data(),
                                    input.size(),
                                    input.type() < FrameType::Voice,
                                    tdoas_.data(),
                                    output.data());
                break;
            case Method::GSC:
                gsc_.DoBeamformimg(input.data(), input.size(), output.data());
                break;
        }
    }

    void ignoreTimeDelays(bool ignore) {
        ignore_toa_ = ignore;
        if (ignore_toa_) {
            std::fill(std::begin(indexes_), std::end(indexes_), 0);
            std::fill(std::begin(tdoas_), std::end(tdoas_), 0);
        }
    }

    void setMargin(std::int32_t margin) {
        if (margin > frames_per_buffer_ / 2) {
            throw std::runtime_error("Maximum allowed margin: "
                                     + std::to_string(frames_per_buffer_ / 2) + "(half of the frames per channels)");
        }
        margin_ = margin;
    }

    bool ignore_toa_{false};
    Method method_{Method::DelayAndSum};
    std::int32_t sample_rate_;
    std::int8_t channels_;
    std::size_t frames_per_buffer_;
    std::size_t nfft_;
    std::int8_t reference_;
    std::int32_t margin_;
    float learning_rate_;
    Mvdr mvdr_;
    Gsc gsc_;
    std::vector<int> indexes_;
    std::vector<float> tdoas_;
};

void score::Beamformer::process(const AudioBuffer &input, AudioBuffer &output) {
    pimpl_->procees(input, output);
}

void score::Beamformer::setMethod(Beamformer::Method method) {
    pimpl_->method_ = method;
}

Beamformer::Method score::Beamformer::method() const {
    return pimpl_->method_;
}

void score::Beamformer::setMargin(std::int32_t margin) {
    pimpl_->setMargin(margin);
}

score::Beamformer::~Beamformer() = default;

score::Beamformer::Beamformer(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_channel,
        float learning_rate, std::int8_t reference) :
      pimpl_(std::make_unique<Pimpl>(sample_rate, channels, frames_per_channel, learning_rate, reference)) {

}

void score::Beamformer::setFFTSize(std::size_t nfft) {
    if (nfft < pimpl_->frames_per_buffer_) {
        throw std::runtime_error("The minimum size should be: " + std::to_string(pimpl_->frames_per_buffer_));
    }
    pimpl_->nfft_ = nextPowerOfTwo(nfft);
}

void score::Beamformer::ignoreTimeDelays(bool ignore) {
    pimpl_->ignoreTimeDelays(ignore);
}

bool score::Beamformer::isIgnoringTimeDelays() const {
    return pimpl_->ignore_toa_;
}
