#include "low_cut_filter.hpp"

using namespace score;

#define WEBRTC_SPL_SAT(a, b, c) (b > a ? a : b < c ? c : b)

namespace {
    const int16_t kFilterCoefficients8kHz[5] = {3798, -7596, 3798, 7807, -3733};
    const int16_t kFilterCoefficients[5] = {4012, -8024, 4012, 8002, -3913};
}

struct BiquadFilter {
public:
    explicit BiquadFilter(int sample_rate_hz)
            : ba_(sample_rate_hz == 8000
                  ? kFilterCoefficients8kHz
                  : kFilterCoefficients) {
        std::memset(x_, 0, sizeof(x_));
        std::memset(y_, 0, sizeof(y_));
    }

    void Process(int16_t* data, size_t length) {
        const int16_t* const ba = ba_;
        int16_t* x = x_;
        int16_t* y = y_;
        int32_t tmp_int32 = 0;

        for (size_t i = 0; i < length; i++) {
            //  y[i] = b[0] * x[i] +  b[1] * x[i-1] +  b[2] * x[i-2]
            //                     + -a[1] * y[i-1] + -a[2] * y[i-2];

            tmp_int32 = y[1] * ba[3];   // -a[1] * y[i-1] (low part)
            tmp_int32 += y[3] * ba[4];  // -a[2] * y[i-2] (low part)
            tmp_int32 = (tmp_int32 >> 15);
            tmp_int32 += y[0] * ba[3];  // -a[1] * y[i-1] (high part)
            tmp_int32 += y[2] * ba[4];  // -a[2] * y[i-2] (high part)
            tmp_int32 *= 2;

            tmp_int32 += data[i] * ba[0];  // b[0] * x[0]
            tmp_int32 += x[0] * ba[1];     // b[1] * x[i-1]
            tmp_int32 += x[1] * ba[2];     // b[2] * x[i-2]

            // Update state (input part).
            x[1] = x[0];
            x[0] = data[i];

            // Update state (filtered part).
            y[2] = y[0];
            y[3] = y[1];
            y[0] = static_cast<int16_t>(tmp_int32 >> 13);

            y[1] = static_cast<int16_t>((tmp_int32 & 0x00001FFF) * 4);

            // Rounding in Q12, i.e. add 2^11.
            tmp_int32 += 2048;

            // Saturate (to 2^27) so that the HP filtered signal does not overflow.
            tmp_int32 = WEBRTC_SPL_SAT(static_cast<int32_t>(134217727), tmp_int32,
                                       static_cast<int32_t>(-134217728));

            // Convert back to Q0 and use rounding.
            data[i] = static_cast<int16_t>(tmp_int32 >> 12);
        }
    }

private:
    const int16_t* const ba_ = nullptr;
    int16_t x_[2]{};
    int16_t y_[4]{};
};


struct LowCutFilter::Pimpl {

    Pimpl(std::int32_t sample_rate, std::int8_t channels) :
        sample_rate_(sample_rate), channels_(channels), filters_(channels, BiquadFilter(sample_rate)) {

    }

    void process(const score::AudioBuffer &input, score::AudioBuffer &output) {

        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected "
                                        + std::to_string(sample_rate_) + " Hz");
        }

        if (channels_ != input.channels()) {
            throw std::invalid_argument("The LowCutOff is configure to work with "
                                        + std::to_string(channels_) + " channels.");
        }

        output.setSampleRate(sample_rate_);
        output.updateRaw(input.channels(), input.framesPerChannel(), input.interleave());
        for (auto i = 0ul; i < channels_; ++i) {
            filters_[i].Process(output.channel(i), output.framesPerChannel());
        }
    }
private:
    std::int8_t channels_;
    std::int32_t sample_rate_;
    std::vector<BiquadFilter> filters_;
};


score::LowCutFilter::LowCutFilter(std::int32_t sample_rate, std::int8_t channels) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels)){

}

void score::LowCutFilter::process(const score::AudioBuffer &input, score::AudioBuffer &output) {
    pimpl_->process(input, output);
}

score::LowCutFilter::~LowCutFilter() = default;
