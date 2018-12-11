
#include "audio_buffer.hpp"
#include "band_extractor.hpp"
#include <limits>
#include <audio_buffer.hpp>


using namespace score;

AudioBuffer::AudioBuffer(float sample_rate) : sample_rate_(sample_rate), fixed_data_still_valid_(true) {

}

AudioBuffer::AudioBuffer(float sample_rate, std::size_t channels, std::size_t frames_per_channel) :
    AudioBuffer(sample_rate) {
    resize(channels, frames_per_channel);
}

AudioBuffer::AudioBuffer(float sample_rate, std::size_t channels, std::size_t frames_per_channel, const int16_t *raw) :
    AudioBuffer(sample_rate, channels, frames_per_channel) {
    updateRaw(channels, frames_per_channel, raw);
}

void AudioBuffer::updateRaw(std::size_t channels, std::size_t frames_per_channel, const int16_t *raw) {
    resize(channels, frames_per_channel);

    for (auto i = 0ul; i < channels; ++i) {
        for (auto j = 0ul, index = i; j < frames_per_channel; ++j, index += channels_) {
            raw_data_[index] = raw[index];
            fixed_data_[i][j] = raw[index];
        }
    }

    fixed_data_still_valid_ = true;
    invalidate_fixed_data(false, true, true);
    invalidate_floating_data(true, true, true);
}



void score::AudioBuffer::resize(std::size_t channels, std::size_t frames_per_channel) {
    channels_ = channels;
    frames_ = frames_per_channel;
    raw_data_.resize(channels * frames_per_channel);
    fixed_data_.resize(channels, std::vector<std::int16_t>(frames_per_channel));
    floating_data_.resize(channels, std::vector<float>(frames_per_channel));
    fixed_mixed_.resize(frames_per_channel);
    floating_mixed_.resize(frames_per_channel);
    band_extractor_.resize(channels);
}

void AudioBuffer::split_helper() {
    if (fixed_bands_still_valid_)
        return;

    fixed_bands_.resize(channels_);
    for (auto& c : fixed_bands_) {
        c.resize(Bands::NumberBands);
        for (auto& b : c) {
            b.resize(frames_);
        }
    }

    for (auto i = 0ul; i < channels_; ++i) {
        band_extractor_[i].process(fixed_data_[i], fixed_bands_[i]);
    }

    fixed_bands_still_valid_ = true;
}

void AudioBuffer::split_helper_f() {
    if (floating_bands_still_valid_)
        return;

    start_fixed_operation(true); // Refresh the bands, just in case.
    floating_bands_.resize(channels_);
    for (auto& c : floating_bands_) {
        c.resize(Bands::NumberBands);
        for (auto& b : c) {
            b.resize(frames_);
        }
    }

    for (auto i = 0ul; i < channels_; ++i) {
        for (auto j = 0ul; j < fixed_bands_.size(); ++j) {
            for (auto z = 0ul; z < frames_; ++z) {
                floating_bands_[i][j][z] = fixed_bands_[i][j][z];
            }
        }
    }

    floating_bands_still_valid_ = true;
}

void AudioBuffer::mixed_helper(Bands band_index) {
    if (fixed_mixed_still_valid_)
        return;

    fixed_mixed_.resize(frames_);
    for (auto i = 0ul; i < channels_; ++i) {
        const auto& b = this->band(i, band_index);
        for (auto j = 0ul; j < frames_; ++j) {
            fixed_mixed_[j] += (b[j] / channels_);
        }
    }

    fixed_mixed_still_valid_ = true;
}

void AudioBuffer::mixed_helper_f(Bands band_index) {
    if (floating_mixed_still_valid_)
        return;

    floating_mixed_.resize(frames_);
    for (auto i = 0ul; i < channels_; ++i) {
        const auto& b = this->band(i, band_index);
        for (auto j = 0ul; j < frames_; ++j) {
            floating_mixed_[j] += (b[j] / static_cast<float>(channels_));
        }
    }

    floating_mixed_still_valid_ = true;
}

std::vector<std::int16_t> &AudioBuffer::channel(std::size_t channel) {
    start_fixed_operation_and_invalidate();
    return fixed_data_[channel];
}

const std::vector<std::int16_t> &AudioBuffer::channel(std::size_t channel) const {
    start_fixed_operation();
    return fixed_data_[channel];
}

std::vector<float> &AudioBuffer::channel_f(std::size_t channel) {
    start_floating_operation_and_invalidate();
    return floating_data_[channel];
}

const std::vector<float> &AudioBuffer::channel_f(std::size_t channel) const {
    start_floating_operation();
    return floating_data_[channel];
}

std::vector<std::int16_t> &AudioBuffer::band(std::size_t channel, Bands band) {
    start_fixed_operation_and_invalidate(true);
    return fixed_bands_[channel][band];
}

const std::vector<std::int16_t> &AudioBuffer::band(std::size_t channel, Bands band) const {
    start_fixed_operation(true);
    return fixed_bands_[channel][band];
}

std::vector<float> &AudioBuffer::band_f(std::size_t channel, Bands band) {
    start_floating_operation_and_invalidate(true);
    return floating_bands_[channel][band];
}

const std::vector<float> &AudioBuffer::band_f(std::size_t channel, Bands band) const {
    start_floating_operation(true);
    return floating_bands_[channel][band];
}

std::vector<std::int16_t> &AudioBuffer::operator[](std::size_t channel) {
    start_fixed_operation_and_invalidate();
    return fixed_data_[channel];
}

const std::vector<std::int16_t> &AudioBuffer::operator[](std::size_t channel) const {
    start_fixed_operation();
    return fixed_data_[channel];
}

void AudioBuffer::merge() {
    start_fixed_operation_and_invalidate(true);
    for (auto i = 0ul; i < channels_; ++i) {
        band_extractor_[i].synthesis(fixed_bands_[i], fixed_data_[i]);
    }
}

float AudioBuffer::sampleRate() const {
    return sample_rate_;
}


std::size_t AudioBuffer::framesPerChannel() const {
    return frames_;
}

std::size_t AudioBuffer::channels() const {
    return channels_;
}

std::size_t AudioBuffer::numberBands() const {
    return Bands::NumberBands;
}

typedef std::numeric_limits<int16_t> limits_int16;

static inline int16_t FloatS16ToS16(float v) {
    static const float kMaxRound = limits_int16::max() - 0.5f;
    static const float kMinRound = limits_int16::min() + 0.5f;
    if (v > 0)
        return v >= kMaxRound ? limits_int16::max()
                              : static_cast<int16_t>(v + 0.5f);
    return v <= kMinRound ? limits_int16::min() : static_cast<int16_t>(v - 0.5f);
}

void AudioBuffer::refresh_fixed() {
    if (fixed_data_still_valid_)
        return;

    for (auto i = 0ul; i < channels_; ++i) {
        for (auto j = 0ul; j < frames_; ++j) {
            fixed_data_[i][j] = FloatS16ToS16(floating_data_[i][j]);
        }
    }

    fixed_bands_still_valid_ = true;
}

void AudioBuffer::refresh_floating() {
    if (floating_data_still_valid_)
        return;

    for (auto i = 0ul; i < channels_; ++i) {
        for (auto j = 0ul; j < frames_; ++j) {
            floating_data_[i][j] = fixed_data_[i][j];
        }
    }

    floating_data_still_valid_ = true;
}

void AudioBuffer::start_fixed_operation(bool update_bands, bool update_mixed) const {
    auto* editable = const_cast<AudioBuffer*>(this);
    editable->refresh_fixed();

    if (update_bands && !fixed_bands_still_valid_) {
        editable->split_helper();
    }

    if (update_mixed && !fixed_mixed_still_valid_) {
        editable->mixed_helper(Band0To8kHz);
    }
}

void AudioBuffer::start_floating_operation(bool update_bands, bool update_mixed) const {
    auto* editable = const_cast<AudioBuffer*>(this);
    editable->refresh_fixed();

    if (update_bands && !floating_bands_still_valid_) {
        editable->split_helper_f();
    }

    if (update_mixed && !floating_mixed_still_valid_) {
        editable->mixed_helper_f(Band0To8kHz);
    }
}


void AudioBuffer::start_fixed_operation_and_invalidate(bool update_bands, bool update_mixed) {
    const auto edit_data = !fixed_data_still_valid_;
    const auto edit_bands = update_bands && !fixed_bands_still_valid_;
    const auto edit_mixed = update_mixed && !fixed_mixed_still_valid_;
    refresh_fixed();

    if (edit_bands) {
        split_helper_f();
    }

    if (edit_mixed) {
        mixed_helper_f(Band0To8kHz);
    }

    invalidate_floating_data(edit_data, edit_bands, edit_mixed);
}

void AudioBuffer::start_floating_operation_and_invalidate(bool update_bands, bool update_mixed) {
    const auto edit_data = !floating_data_still_valid_;
    const auto edit_bands = update_bands && !floating_bands_still_valid_;
    const auto edit_mixed = update_mixed && !floating_mixed_still_valid_;
    refresh_floating();

    if (edit_bands) {
        split_helper_f();
    }

    if (edit_bands) {
        mixed_helper_f(Band0To8kHz);
    }

    invalidate_fixed_data(edit_data, edit_bands, edit_mixed);
}

void AudioBuffer::invalidate_fixed_data(bool data_edited, bool bands_edited, bool mixed_edited) {
    fixed_bands_still_valid_ = !bands_edited;
    fixed_data_still_valid_ = !data_edited;
    fixed_mixed_still_valid_ = !mixed_edited;
}

void AudioBuffer::invalidate_floating_data(bool data_edited, bool bands_edited, bool mixed_edited) {
    floating_data_still_valid_ = !data_edited;
    floating_bands_still_valid_ = !bands_edited;
    floating_mixed_still_valid_ = !mixed_edited;
}

const std::vector<std::int16_t> &AudioBuffer::downmix() const {
    start_fixed_operation(true, true);
    return fixed_mixed_;
}

const std::vector<float> &AudioBuffer::downmix_f() const {
    start_floating_operation(true, true);
    return floating_mixed_;
}

const std::vector<std::int16_t> &AudioBuffer::raw() const {
    return raw_data_;
}

std::vector<std::int16_t> &AudioBuffer::raw() {
    return raw_data_;
}

double AudioBuffer::timestamp() const {
    return timestamp_;
}

void AudioBuffer::setTimestamp(double timestamp) {
    timestamp_ = timestamp;
}

std::size_t AudioBuffer::size() const {
    return channels_ * frames_;
}


