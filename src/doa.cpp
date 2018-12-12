#include "doa.hpp"
#include "utils.hpp"

#include <eigen3/unsupported/Eigen/FFT>
#include <complex>
#include <algorithm>


using namespace score;
struct DOA::Pimpl {

    explicit Pimpl(std::int32_t sample_rate, float microphone_distances = 0.08127, float sound_speed = 343.2f) :
            maximum_tau_(microphone_distances / sound_speed),
            sample_rate_(sample_rate),
            microphone_distances_(microphone_distances),
            sound_speed_(sound_speed) {

    }

    ~Pimpl() {
        reset();
    }

    void reset() {

    }

    float gccPhat(const std::int16_t* signal, const std::int16_t* reference, size_t size) {
        const auto expected_size = 2 * size;
        if (expected_size != fft_size_) {
            reset();
            fft_size_ = (size_t) expected_size;
            signal_.resize(expected_size, 0);
            signal_spectrum_.resize(expected_size);
            reference_.resize(expected_size, 0);
            reference_.resize(expected_size);
            gcc_.resize(expected_size);
        }

        Converter::S16ToFloat(signal, size, signal_.data());
        Converter::S16ToFloat(reference, size, reference_.data());


        fft_.fwd(signal_spectrum_, signal_);
        fft_.fwd(reference_spectrum_ , reference_);
        std::transform(signal_spectrum_.begin(), signal_spectrum_.end(),
                reference_spectrum_.begin(), signal_spectrum_.begin(), [](const auto& s, const auto& r) {
            const auto operation = s * std::conj(r);
            return operation / std::abs(operation);
        });
        fft_.inv(gcc_, signal_spectrum_);

        const auto maximum_tau_index = static_cast<std::size_t >(sample_rate_ * maximum_tau_);
        const auto max_shift = std::min(expected_size / 2, maximum_tau_index);
        const auto lambda = [](const auto left, const auto right) { return std::abs(left) < std::abs(right); };
        const auto maximum_left = std::max_element(gcc_.begin(), gcc_.begin() + max_shift, lambda);
        const auto maximum_right = std::max_element(gcc_.end() - max_shift, gcc_.end(), lambda);
        const auto shift = std::distance(gcc_.begin(),
                *maximum_left > *maximum_right ? maximum_left : maximum_right) - max_shift;

        return shift / sample_rate_;
    }


    float computeDOA() {
        const auto min_index = std::distance(tau_.begin(), std::min_element(tau_.begin(), tau_.end()));
        auto best_guess = 0;
        if ((min_index != 0 and theta_[min_index - 1] >= 0) or (min_index == 0 and theta_.back() < 0)) {
            best_guess = (theta_[min_index] + 360) % 360;
        } else {
            best_guess = (180 - theta_[min_index]);
        }

        return (best_guess + 120 + min_index * 60) % 360;
    }

    float process(const AudioBuffer& microphone_inputs) {
        for (auto i = 0ul, size = tau_.size(); i < size; ++i) {
            const auto group = microphone_groups_[i];
            tau_[i] = gccPhat(microphone_inputs.channel(group.first),
                    microphone_inputs.channel(group.second),
                    microphone_inputs.framesPerChannel());
            theta_[i] = static_cast<int>(std::asin(tau_[i] / maximum_tau_) * 180.0f / M_PI);
        }

        return computeDOA();
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
    std::int32_t sample_rate_{};
    float maximum_tau_{};
    float microphone_distances_{};
    float sound_speed_{};
    Eigen::FFT<float> fft_;
};

DOA::DOA(std::int32_t sample_rate, float microphone_distances, float sound_speed) :
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

void DOA::setSampleRate(std::int32_t sample_rate) {
    pimpl_->sample_rate_ = sample_rate;
}

float DOA::sampleRate() const {
    return pimpl_->sample_rate_;
}

void DOA::reset() {

}

DOA::~DOA() = default;
