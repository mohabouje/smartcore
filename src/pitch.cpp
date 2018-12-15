#include "pitch.hpp"
#include <unordered_map>
#include <aubio/aubio.h>

using namespace score;

struct Pitch::Pimpl {

    Pimpl(std::int32_t sample_rate, Method method, std::size_t frame_size, std::size_t hope_size) :
        sample_rate_(sample_rate),
        method_(method) {
        tracker_ = new_aubio_pitch(EquivalentMethod.find(method)->second, frame_size, hope_size, sample_rate);
        if (tracker_ == nullptr) {
            throw std::bad_alloc();
        }
        out_ = new_fvec(1);
    }

    ~Pimpl() {
        del_aubio_pitch(tracker_);
    }

    void setUnit(FrequencyUnit unit) {
        unit_ = unit;
        aubio_pitch_set_unit(tracker_, EquivalentUnit.find(unit)->second);
    }

    FrequencyUnit unit() const {
        return unit_;
    }

    void setTolerance(float tolerance) {
        aubio_pitch_set_tolerance(tracker_, tolerance);
    }

    float tolerance() const {
        return aubio_pitch_get_tolerance(tracker_);
    }

    void setSilenceLevel(float silence_level_db) {
        aubio_pitch_set_silence(tracker_, silence_level_db);
    }


    float silenceLevel() const {
        return aubio_pitch_get_silence(tracker_);
    }


    std::pair<float, float> process(const AudioBuffer& input) {
        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected "
                                        + std::to_string(sample_rate_) + " Hz");
        }

        if (1 != input.channels()) {
            throw std::invalid_argument("The PitchTracker is configure to work with "
                                        + std::to_string(1) + " channels.");
        }


        const fvec_t in = {(unsigned int)input.size(), const_cast<float*>(input.data())};
        aubio_pitch_do(tracker_, &in, out_);

        return {out_->data[0], aubio_pitch_get_confidence(tracker_)};
    }

private:
    fvec_t* out_;
    std::int32_t sample_rate_;
    aubio_pitch_t* tracker_{nullptr};
    FrequencyUnit unit_{FrequencyUnit::Hz};
    Method method_;
    const std::unordered_map<Pitch::Method, const char*> EquivalentMethod = {
            {Method::SchmittTrigger, "smitth"},
            {Method::FastHarmonicCombFilter, "fcomb"},
            {Method::MultipleCombFilter, "mcomb"},
            {Method::Yin, "yin"},
            {Method::YinFast, "yinfast"},
            {Method::YinFFT, "yinfft"}
    };
    const std::unordered_map<FrequencyUnit, const char*> EquivalentUnit = {
            {FrequencyUnit::Hz, "Hz"},
            {FrequencyUnit::Midi, "midi"},
            {FrequencyUnit::Cent, "cent"},
            {FrequencyUnit::Bin, "bin"}

    };
};


score::Pitch::Pitch(std::int32_t sample_rate, Pitch::Method method, std::size_t frame_size) :
    Pitch(sample_rate, method, frame_size, frame_size / 2) {

}

score::Pitch::Pitch(std::int32_t sample_rate, Pitch::Method method, std::size_t frame_size, std::size_t hope_size) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, method, frame_size, hope_size)) {

}

score::Pitch::~Pitch() = default;

void score::Pitch::setUnit(FrequencyUnit unit) {
    pimpl_->setUnit(unit);
}

FrequencyUnit score::Pitch::unit() const {
    return pimpl_->unit();
}

void score::Pitch::setTolerance(float tolerance) {
    pimpl_->setTolerance(tolerance);
}

float score::Pitch::tolerance() const {
    return pimpl_->tolerance();
}

void score::Pitch::setSilenceLevel(float silence_level_db) {
    pimpl_->setSilenceLevel(silence_level_db);
}

float score::Pitch::silenceLevel() const {
    return pimpl_->silenceLevel();
}

std::pair<float, float> score::Pitch::process(const AudioBuffer &input) {
    return pimpl_->process(input);
}
