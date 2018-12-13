#include "noise_level.hpp"
#include "utils.hpp"

#include <algorithm>
#include <numeric>
#include <cmath>

using namespace score;

struct NoiseLevel::Pimpl {

    Pimpl(std::int32_t sample_rate, std::size_t frame_length) :
        cast_data_(frame_length),
        sample_rate_(sample_rate),
        frame_length_(frame_length)
    {
        const auto number_frames_per_second = static_cast<float>(sample_rate) / frame_length;
        min_noise_energy_ = static_cast<float>(sample_rate) * 2.f * 2.f / number_frames_per_second;
    }

    float computeEnergy(const AudioBuffer& input) {
        auto energy = 0.0f;
        for (auto i = 0; i < input.channels(); ++i) {
            Converter::S16ToFloatS16(input.channel(i), input.framesPerChannel(), cast_data_.data());
            const auto channel_energy =  std::accumulate(cast_data_.begin(), cast_data_.end(), 0.f,
                                    [](auto a, auto b) { return a + b * b; });
            energy = std::max(energy, channel_energy);
        }
        return energy;
    }

    void reset() {
        reset_ = true;
        noise_energy_hold_counter_ = 0;
    }

    float rms(float energy) {
        return std::sqrt(energy / static_cast<float>(frame_length_));
    }


    float process(const AudioBuffer& input) {

        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Expected an input frame at " + std::to_string(sample_rate_) + " Hz.");
        }

        if (input.framesPerChannel() != frame_length_) {
            throw std::invalid_argument("Expected an input frame of " + std::to_string(frame_length_) + " samples.");
        }

        const auto frame_energy = computeEnergy(input);
        if (frame_energy <= 0.f) {
            return rms(noise_energy_);
        }

        if (reset_) {
            reset_ = false;
            return rms(std::max(frame_energy, noise_energy_));
        }

        // TODO call a signal classifier!
        const auto stationary = true;
        if (stationary) {
            if (frame_energy > noise_energy_) {
                // Leak the estimate upwards towards the frame energy if no recent downward update.
                noise_energy_hold_counter_ = std::max(noise_energy_hold_counter_ - 1, 0);
                if (noise_energy_hold_counter_ == 0) {
                    noise_energy_ = std::min(noise_energy_ * 1.01f, frame_energy);
                }

            } else {
                // Update smoothly downwards with a limited maximum update magnitude.
                noise_energy_ = std::max(noise_energy_ * 0.9f, noise_energy_ + 0.05f * (frame_energy - noise_energy_));
                noise_energy_hold_counter_ = 1000;
            }
        } else {
            // For a non-stationary signal, leak the estimate downwards in order to
            // avoid estimate locking due to incorrect signal classification.
            noise_energy_ *= 0.99f;
        }
        noise_energy_ = std::max(noise_energy_, min_noise_energy_);
        return rms(noise_energy_);
    }

    std::vector<float> cast_data_;
    std::int32_t sample_rate_;
    std::size_t frame_length_;
    int noise_energy_hold_counter_{0};
    float noise_energy_{1.f};
    float min_noise_energy_{0.0};
    bool reset_{true};
};

score::NoiseLevel::NoiseLevel(std::int32_t sample_rate, std::size_t frame_length) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, frame_length)) {

}

float score::NoiseLevel::process(const AudioBuffer &input) {
    return pimpl_->process(input);
}

float score::NoiseLevel::noiseLevel() const {
    return pimpl_->noise_energy_;
}

score::NoiseLevel::~NoiseLevel() = default;
