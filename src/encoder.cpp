#include "encoder.hpp"
#include <sndfile.h>

using namespace score;


struct Encoder::Pimpl {

    Pimpl(const std::string& file, std::int32_t sample_rate, std::int8_t channels) {
        info_.samplerate = sample_rate;
        info_.channels = channels;
        info_.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

        file_ = sf_open(file.c_str(), SFM_WRITE, &info_);
        if (file_ == nullptr) {
            throw std::runtime_error(sf_strerror(file_));
        }
    }

    ~Pimpl() {
        sf_close(file_);
    }

    void process(const AudioBuffer& buffer) {
        temporal_.resize(buffer.size());
        buffer.toInterleave(temporal_.data());

        const auto samples = sf_write_float(file_, temporal_.data(), temporal_.size());
        if (samples != buffer.size()) {
            throw std::runtime_error("Error while encoding buffer. Encoded samples: " + std::to_string(samples)
            + "/" + std::to_string(buffer.size()));
        }
        sf_write_sync(file_);

    }

private:
    std::vector<float> temporal_;
    SF_INFO info_{};
    SNDFILE* file_;
};

score::Encoder::Encoder(const std::string &file, std::int32_t sample_rate, std::int8_t channels) :
    pimpl_(std::make_unique<Pimpl>(file, sample_rate, channels)) {
}

void score::Encoder::process(const AudioBuffer &input) {
    pimpl_->process(input);
}

score::Encoder::~Encoder() = default;
