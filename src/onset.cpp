#include "onset.hpp"
#include <unordered_map>
#include <aubio/aubio.h>
#include <onset.hpp>


using namespace score;

struct Onset::Pimpl {

    Pimpl(std::int32_t sample_rate, Method method, std::size_t frame_size, std::size_t hope_size) :
            sample_rate_(sample_rate),
            method_(method) {
        tracker_ = new_aubio_onset(EquivalentMethod.find(method)->second, frame_size, hope_size, sample_rate);
        if (tracker_ == nullptr) {
            throw std::bad_alloc();
        }
        out_ = new_fvec(1);
    }

    ~Pimpl() {
        del_aubio_onset(tracker_);
    }

    void enableAdaptiveWhitening(bool state) {
       aubio_onset_set_awhitening(tracker_, static_cast<uint_t>(state));
    }

    bool isAdaptiveWhiteningEnabled() const {
        return static_cast<bool>(aubio_onset_get_awhitening(tracker_));
    }

    float logCompression() const {
        return aubio_onset_get_compression(tracker_);
    }

    void setLogCompression(float log_compression) {
        aubio_onset_set_compression(tracker_, log_compression);
    }

    void setSilenceLevel(float silence_level_db) {
        aubio_onset_set_silence(tracker_, silence_level_db);
    }

    float silenceLevel() const {
        return aubio_onset_get_silence(tracker_);
    }

    float descriptor() const {
        return aubio_onset_get_descriptor(tracker_);
    }

    float thresholdedDescriptor() const {
        return aubio_onset_get_thresholded_descriptor(tracker_);
    }

    void setThreshold(float threshold) {
        aubio_onset_set_threshold(tracker_, threshold);
    }

    float threshold() const {
        return aubio_onset_get_threshold(tracker_);
    }

    void setMinimumInterOnset(float minioi_msecs) {
        aubio_onset_set_minioi_ms(tracker_, minioi_msecs);
    }

    float minimumInterOnset() const {
        return aubio_onset_get_minioi_ms(tracker_);
    }

    void setDelay(float delay_msecs) {
        aubio_onset_set_delay_ms(tracker_, delay_msecs);
    }

    float delay() const {
        return aubio_onset_get_delay_ms(tracker_);
    }

    void reset() {
        aubio_onset_reset(tracker_);
    }

    std::pair<bool, float> process(const AudioBuffer& input) {
        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected "
                                        + std::to_string(sample_rate_) + " Hz");
        }

        if (1 != input.channels()) {
            throw std::invalid_argument("The PitchTracker is configure to work with "
                                        + std::to_string(1) + " channels.");
        }


        const fvec_t in = {(unsigned int)input.size(), const_cast<float*>(input.data())};
        aubio_onset_do(tracker_, &in, out_);


        return {out_->data[0] > 0, aubio_onset_get_last_ms(tracker_)};
    }

private:
    aubio_onset_t* tracker_;
    fvec_t* out_;
    float sample_rate_;
    Method method_;
    const std::unordered_map<Onset::Method, const char*> EquivalentMethod = {
            {Method::Energy,               "energy"},
            {Method::HighFrequencyContent, "hfc"},
            {Method::ComplexDomain,        "complex"},
            {Method::PhaseDomain,          "phase"},
            {Method::WeightedPhase,          "wphase"},
            {Method::SpectralDifference,  "specdiff" },
            {Method::SpectralFlux,         "specflux"},
            {Method::KullbackLiebler,         "kl"},
            {Method::ModifiedKullbakLiebler,         "mkl"}
    };
};

score::Onset::Onset(std::int32_t sample_rate, Onset::Method method, std::size_t frame_size) :
    Onset(sample_rate, method, frame_size, frame_size / 2) {

}

score::Onset::Onset(std::int32_t sample_rate, Onset::Method method, std::size_t frame_size, std::size_t hope_size) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, method, frame_size, hope_size)) {

}

score::Onset::~Onset() = default;

void score::Onset::enableAdaptiveWhitening(bool state) {
    pimpl_->enableAdaptiveWhitening(state);
}

bool score::Onset::isAdaptiveWhiteningEnabled() const {
    return pimpl_->isAdaptiveWhiteningEnabled();
}

float score::Onset::logCompression() const {
    return pimpl_->logCompression();
}

void score::Onset::setLogCompression(float log_compression) {
    pimpl_->setLogCompression(log_compression);
}

void score::Onset::setSilenceLevel(float silence_level_db) {
    pimpl_->setSilenceLevel(silence_level_db);
}

float score::Onset::silenceLevel() const {
    return pimpl_->silenceLevel();
}

float score::Onset::descriptor() const {
    return pimpl_->descriptor();
}

float score::Onset::thresholdedDescriptor() const {
    return pimpl_->thresholdedDescriptor();
}

void score::Onset::setThreshold(float threshold) {
    pimpl_->setThreshold(threshold);
}

float score::Onset::threshold() const {
    return pimpl_->threshold();
}

void score::Onset::setMinimumInterOnset(float minioi_msecs) {
    pimpl_->setMinimumInterOnset(minioi_msecs);
}

float score::Onset::minimumInterOnset() const {
    return pimpl_->minimumInterOnset();
}

void score::Onset::setDelay(float delay_msecs) {
    pimpl_->setDelay(delay_msecs);
}

float score::Onset::delay() const {
    return pimpl_->delay();
}

std::pair<bool, float> score::Onset::process(const AudioBuffer &input) {
    return pimpl_->process(input);
}

void score::Onset::reset() {
    pimpl_->reset();
}
