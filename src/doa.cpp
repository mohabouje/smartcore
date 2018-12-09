#include "doa.hpp"

#include <fftw3.h>
#include <complex>
#include <algorithm>
#include <doa.hpp>


using namespace score;
struct DOA::Pimpl {

    Pimpl(float sample_rate, float microphone_distances = 0.08127, float sound_speed = 343.2f) :
            maximum_tau_(microphone_distances / sound_speed),
            sample_rate_(sample_rate),
            microphone_distances_(microphone_distances),
            sound_speed_(sound_speed) {

    }

    ~Pimpl() {
        reset();
    }

    void reset() {
        if (signal_plan_ != nullptr) {
            fftwf_destroy_plan(signal_plan_);
            fftwf_destroy_plan(reference_plan_);
            fftwf_destroy_plan(gcc_plan_);
        }
    }

    float gccPhat(const std::vector<float>& signal, const std::vector<float>& reference) {
        const auto expected_size = 2 * signal.size();
        if (expected_size != fft_size_) {
            reset();

            fft_size_ = (size_t) expected_size;
            signal_.resize(expected_size);
            signal_spectrum_.resize(expected_size);
            reference_.resize(expected_size);
            reference_.resize(expected_size);
            gcc_.resize(expected_size);

            signal_plan_ = fftwf_plan_dft_r2c_1d((int) expected_size, signal_.data(),
                    reinterpret_cast<fftwf_complex*>(signal_spectrum_.data()), FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);
            reference_plan_ = fftwf_plan_dft_r2c_1d((int) expected_size, reference_.data(),
                    reinterpret_cast<fftwf_complex*>(reference_spectrum_.data()), FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);
            gcc_plan_ = fftwf_plan_dft_c2r_1d((int) expected_size,
                    reinterpret_cast<fftwf_complex*>(gcc_spectrum_.data()),
                    gcc_.data(), FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);
        }

        std::copy(signal.begin(), signal.end(), signal_.data());
        std::fill(signal_.begin() + signal.size(), signal_.end(), 0.0f);

        std::copy(reference.begin(), reference.end(), reference_.data());
        std::fill(reference_.begin() + reference.size(), reference_.end(), 0.0f);

        fftwf_execute(signal_plan_);
        fftwf_execute(reference_plan_);

        std::transform(signal_spectrum_.begin(), signal_spectrum_.end(),
                reference_spectrum_.begin(), gcc_spectrum_.begin(), [](const auto& signal, const auto& reference) {
            const auto operation = signal * std::conj(reference);
            return operation / std::abs(operation);
        });

        fftwf_execute(gcc_plan_);

        const auto maximum_tau_index = static_cast<std::size_t >(sample_rate_ * maximum_tau_);
        const auto max_shift = std::min(expected_size / 2, maximum_tau_index);
        const auto lambda = [](const auto left, const auto right) { return std::abs(left) < std::abs(right); };
        const auto maximum_left = std::max_element(gcc_.begin(), gcc_.begin() + max_shift, lambda);
        const auto maximum_right = std::max_element(gcc_.end() - max_shift, gcc_.end(), lambda);
        const auto shift = std::distance(gcc_.begin(),
                *maximum_left > *maximum_right ? maximum_left : maximum_right) - max_shift;

        return shift / sample_rate_;
    }


    float process(const AudioBuffer& microphone_inputs) {

        for (auto i = 0ul, size = tau_.size(); i < size; ++i) {
            const auto group = microphone_groups_[i];
            tau_[i] = gccPhat(microphone_inputs.channel_f(group.first), microphone_inputs.channel_f(group.second));
            theta_[i] = static_cast<int>(std::asin(tau_[i] / maximum_tau_) * 180.0f / M_PI);
        }

        const auto min_index = std::distance(tau_.begin(), std::min_element(tau_.begin(), tau_.end()));
        auto best_guess = 0;
        if ((min_index != 0 and theta_[min_index - 1] >= 0) or (min_index == 0 and theta_.back() < 0)) {
            best_guess = (theta_[min_index] + 360) % 360;
        } else {
            best_guess = (180 - theta_[min_index]);
        }

        return (best_guess + 120 + min_index * 60) % 360;
    }

    void setGroupMicrophones(const std::vector<std::pair<std::size_t, std::size_t>>& microphone_groups) {
        microphone_groups_ = microphone_groups;
        tau_.resize(microphone_groups.size());
        theta_.resize(microphone_groups.size());
    }

    std::vector<float> signal_{};
    std::vector<float> reference_{};
    std::vector<float> gcc_{};
    std::vector<float> tau_;
    std::vector<int> theta_;
    std::vector<std::complex<float>> signal_spectrum_{};
    std::vector<std::complex<float>> reference_spectrum_{};
    std::vector<std::complex<float>> gcc_spectrum_{};
    std::vector<std::pair<std::size_t, std::size_t>> microphone_groups_{};
    std::size_t fft_size_{0};
    float maximum_tau_{};
    float sample_rate_{};
    float microphone_distances_{};
    float sound_speed_{};
    fftwf_plan signal_plan_{nullptr};
    fftwf_plan reference_plan_{nullptr};
    fftwf_plan gcc_plan_{nullptr};
};

DOA::DOA(float sample_rate, float microphone_distances, float sound_speed) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, microphone_distances, sound_speed)){

}

void DOA::setGroupMicrophones(const std::vector<std::pair<std::size_t, std::size_t>>& microphone_groups) {
    pimpl_->setGroupMicrophones(microphone_groups);
}

void DOA::setSoundSpeed(float sound_speed) {
    pimpl_->sound_speed_ = sound_speed;
}

float DOA::soundSpeed() const {
    return pimpl_->sound_speed_;
}

const std::vector<std::pair<std::size_t, std::size_t>> &score::DOA::groupMicrophones() const {
    return pimpl_->microphone_groups_;
}

float DOA::process(const AudioBuffer &microphone_inputs) {
    return pimpl_->process(microphone_inputs);
}

void DOA::setSampleRate(float sample_rate) {
    pimpl_->sample_rate_ = sample_rate;
}

float DOA::sampleRate() const {
    return pimpl_->sample_rate_;
}

void DOA::reset() {

}

DOA::~DOA() = default;
