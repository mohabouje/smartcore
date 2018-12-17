#include <gtest/gtest.h>

#include <decoder.hpp>
#include <delay.hpp>

#include <edsp/spectral/dft.hpp>
#include <edsp/io/decoder.hpp>
#include <vector>
#include <complex>

inline float gccPhat(const float* signal, const float* reference, size_t size,
        float sample_rate, float maximum_tau = std::numeric_limits<float>::max()) {

    std::vector<float> signal_{};
    std::vector<float> reference_{};
    std::vector<float> gcc_{};
    std::vector<float> tau_;
    std::vector<int> theta_;
    std::vector<std::complex<float>> signal_spectrum_{};
    std::vector<std::complex<float>> reference_spectrum_{};

    const auto expected_size = 2 * size;
    const auto nfft = edsp::spectral::make_fft_size(expected_size);
    signal_.resize(expected_size, 0);
    reference_.resize(expected_size, 0);
    gcc_.resize(expected_size, 0);
    signal_spectrum_.resize(nfft, 0);
    reference_spectrum_.resize(nfft, 0);


    std::copy(signal, signal + size, signal_.data());
    std::copy(reference, reference + size, reference_.data());

    edsp::spectral::dft(std::begin(signal_),
                        std::end(signal_), std::begin(signal_spectrum_));
    edsp::spectral::dft(std::begin(reference_),
                        std::end(reference_), std::begin(reference_spectrum_));

    std::transform(signal_spectrum_.begin(), signal_spectrum_.end(),
                   reference_spectrum_.begin(), signal_spectrum_.begin(), [](const auto& s, const auto& r) {
                const auto operation = s * std::conj(r);
                return operation / std::abs(operation);
            });

    edsp::spectral::idft(std::begin(signal_spectrum_), std::end(signal_spectrum_), std::begin(gcc_));

    const auto maximum_tau_index = static_cast<std::size_t >(sample_rate * maximum_tau);
    const auto max_shift = std::min(expected_size / 2, maximum_tau_index);
    const auto lambda = [](const auto left, const auto right) { return std::abs(left) < std::abs(right); };

    const auto maximum_left = std::max_element(gcc_.begin(), gcc_.begin() + max_shift + 1, lambda);
    const auto maximum_right = std::max_element(gcc_.end() - max_shift, gcc_.end(), lambda);

    if (*maximum_left > *maximum_right) {
        const auto shift = std::distance(gcc_.begin(), maximum_left) + max_shift;
        return static_cast<float>(shift - max_shift) / sample_rate;
    } else {
        const auto shift = std::distance(gcc_.end() - max_shift, maximum_right);
        return static_cast<float>(shift - max_shift) / sample_rate;
    }
}


TEST(DOATest, FromFile) {
    constexpr auto DelaySeconds = 0.01;
    auto decoder = std::make_unique<score::Decoder>("/home/mboujemaoui/Documents/male.wav");
    auto delay = std::make_unique<score::Delay>(decoder->sampleRate(), 1000 * DelaySeconds);

    score::AudioBuffer input, delayed;
    decoder->process(input, decoder->sampleRate());
    delay->process(input, delayed);

    for (auto i = 0; i < input.channels(); ++i) {
        const auto estimated_delay = gccPhat(delayed.channel(i), input.channel(i),
                input.framesPerChannel(), decoder->sampleRate(), 0.05);
        EXPECT_NEAR(estimated_delay, DelaySeconds, 0.5);
    }
}