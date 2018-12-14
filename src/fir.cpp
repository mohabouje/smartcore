#include <fir.hpp>
#include <fir_filter_c.h>

using namespace score;

struct FIR::Pimpl {

    Pimpl(std::int8_t channels, const float *coefficients, std::size_t size) :
            coefficients_(coefficients, coefficients + size),
            firs_(channels) {

        for (auto& fir : firs_)
            fir = std::make_unique<webrtc::FIRFilterC>(coefficients, size);

    }

    void process(const AudioBuffer& input, AudioBuffer& output) {
        if (input.channels() != firs_.size()) {
            throw std::invalid_argument("Expected an input frame with "
                                        + std::to_string(firs_.size()) + " channels.");
        }

        output.setSampleRate(input.sampleRate());
        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < input.channels(); ++i) {
            firs_[i]->Filter(input.channel(i), input.framesPerChannel(), output.channel(i));
        }

    }

    std::vector<float> coefficients_;
    std::vector<std::unique_ptr<webrtc::FIRFilterC>> firs_;
};


score::FIR::FIR(std::int8_t channels, const float *coefficients, std::size_t size) :
    pimpl_(std::make_unique<Pimpl>(channels, coefficients, size)) {

}

score::FIR::~FIR() {

}

const float *score::FIR::coefficients() const {
    return pimpl_->coefficients_.data();
}

std::size_t score::FIR::order() const {
    return pimpl_->coefficients_.size();
}

void score::FIR::process(const AudioBuffer &input, AudioBuffer &output) {
    pimpl_->process(input, output);
}
