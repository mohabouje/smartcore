#include "audio_buffer.hpp"
#include <iostream>


using namespace score;

AudioBuffer::AudioBuffer() = default;


AudioBuffer::AudioBuffer(std::int32_t sample_rate) : sample_rate_(sample_rate) {

}

AudioBuffer::AudioBuffer(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_channel) :
    AudioBuffer(sample_rate) {
    resize(channels, frames_per_channel);
}

AudioBuffer::AudioBuffer(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_channel, const int16_t *raw) :
    AudioBuffer(sample_rate, channels, frames_per_channel) {
    fromInterleave(channels, frames_per_channel, raw);
}

void AudioBuffer::fromInterleave(std::int8_t channels, std::size_t frames_per_channel, const int16_t *raw) {
    resize(channels, frames_per_channel);
    for (auto i = 0ul, index = 0ul; i < frames_per_channel; ++i) {
        for (auto j = 0ul; j < channels; ++j, ++index) {
            deinterleaved_data_(j, i) = raw[index];
        }
    }
}

void score::AudioBuffer::resize(std::int8_t channels, std::size_t frames_per_channel) {
    channels_ = channels;
    frames_per_channel_ = frames_per_channel;
    deinterleaved_data_.resize(channels, frames_per_channel);
}


std::int16_t *AudioBuffer::channel(std::size_t channel) {
    return deinterleaved_data_.row(channel).data();
}

const std::int16_t *AudioBuffer::channel(std::size_t channel) const {
    return deinterleaved_data_.row(channel).data();
}

std::int16_t *AudioBuffer::operator[](std::size_t channel) {
    return deinterleaved_data_.row(channel).data();
}

const std::int16_t *AudioBuffer::operator[](std::size_t channel) const {
    return deinterleaved_data_.row(channel).data();
}

std::int32_t AudioBuffer::sampleRate() const {
    return sample_rate_;
}

std::size_t AudioBuffer::framesPerChannel() const {
    return frames_per_channel_;
}

std::int8_t AudioBuffer::channels() const {
    return channels_;
}

double AudioBuffer::timestamp() const {
    return timestamp_;
}

void AudioBuffer::setTimestamp(double timestamp) {
    timestamp_ = timestamp;
}

std::size_t AudioBuffer::size() const {
    return frames_per_channel_ * channels_;
}

void AudioBuffer::setSampleRate(std::int32_t sample_rate) {
    sample_rate_ = sample_rate;
}

size_t AudioBuffer::duration() const {
    return static_cast<std::size_t >(1e3 * frames_per_channel_ / sample_rate_);
}

std::int16_t* AudioBuffer::interleave() const {
    interleaved_data_.resize(channels_ * frames_per_channel_);
    for (auto i = 0ul, index = 0ul; i < frames_per_channel_; ++i) {
        for (auto j = 0ul; j < channels_; ++j, ++index) {
            interleaved_data_[index] = deinterleaved_data_(j, i);
        }
    }
    return interleaved_data_.data();
}

void AudioBuffer::copyTo(AudioBuffer &buffer) const {
    buffer.setSampleRate(buffer.sampleRate());
    if (buffer.channels() != this->channels_ || buffer.framesPerChannel() != this->frames_per_channel_) {
        buffer.resize(this->channels_, this->frames_per_channel_);
    }

    buffer.deinterleaved_data_ = this->deinterleaved_data_;
}

