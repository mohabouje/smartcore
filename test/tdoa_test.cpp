#include "data.h"

#include <tdoa.hpp>
#include <gtest/gtest.h>
#include <edsp/oscillator.hpp>
using namespace score;

static void DelaySequence(float *in_data, int num, int delay, float *out_data) {
    for (int i = 0; i < num; i++) {
        if (i - delay >= 0 && i - delay < num) {
            out_data[i] = in_data[i - delay];
        }
        else {
            out_data[i] = 0;
        }
    }
}

TEST(TDOATest, 3Channels) {
    constexpr auto Channels = 3;
    constexpr auto SamplesPerChannel = 4096;
    constexpr auto SampleRate = 48000.0f;
    constexpr auto Amplitude = 1.0f;
    constexpr auto Frequency = 1000.0f;
    constexpr auto Phase = 0.0f;

    std::vector<int> delays = { 0, 10, -5};

    using sinusoidal = edsp::oscillators::sin_oscillator<float>;
    std::array<float, SamplesPerChannel> temp{};
    std::generate(std::begin(temp), std::end(temp), sinusoidal(Amplitude, SampleRate, Frequency, Phase));

    AudioBuffer input(SampleRate, Channels, SamplesPerChannel);
    for (auto i = 0ul; i < Channels; ++i) {
        DelaySequence(g_data, temp.size(), delays[i], input.channel(i));
    }

    auto tdoa = std::make_unique<TDOA>(SampleRate, Channels, SamplesPerChannel);
    tdoa->setMargin(SamplesPerChannel / 2);

    const auto& results = tdoa->process(input);
    for (auto i = 0; i < Channels; ++i) {
        EXPECT_NEAR(results[i] * SampleRate, delays[i], 0.01);
    }
}

TEST(TDOATest, 5Channels) {
    constexpr auto Channels = 5;
    constexpr auto SamplesPerChannel = 4096;
    constexpr auto SampleRate = 48000.0f;
    constexpr auto Amplitude = 1.0f;
    constexpr auto Frequency = 1000.0f;
    constexpr auto Phase = 0.0f;

    std::vector<int> delays = { 0, -50, -80, -60, 10};

    using sinusoidal = edsp::oscillators::sin_oscillator<float>;
    std::array<float, SamplesPerChannel> temp{};
    std::generate(std::begin(temp), std::end(temp), sinusoidal(Amplitude, SampleRate, Frequency, Phase));

    AudioBuffer input(SampleRate, Channels, SamplesPerChannel);
    for (auto i = 0ul; i < Channels; ++i) {
        DelaySequence(g_data, temp.size(), delays[i], input.channel(i));
    }

    auto tdoa = std::make_unique<TDOA>(SampleRate, Channels, SamplesPerChannel);
    tdoa->setMargin(SamplesPerChannel / 2);

    const auto& results = tdoa->process(input);
    for (auto i = 0; i < Channels; ++i) {
        EXPECT_NEAR(results[i] * SampleRate, delays[i], 0.01);
    }
}

TEST(TDOATest, MultipleChannelsSinusoidal) {
    constexpr auto Channels = 3;
    constexpr auto SamplesPerChannel = 4096;
    constexpr auto SampleRate = 48000.0f;
    constexpr auto Amplitude = 1.0f;
    constexpr auto Frequency = 1000.0f;
    constexpr auto Phase = 0.0f;

    std::vector<int> delays = { 0, -50, -80};

    using sinusoidal = edsp::oscillators::sin_oscillator<float>;
    std::array<float, SamplesPerChannel> temp{};
    std::generate(std::begin(temp), std::end(temp), sinusoidal(Amplitude, SampleRate, Frequency, Phase));

    AudioBuffer input(SampleRate, Channels, SamplesPerChannel);
    for (auto i = 0ul; i < Channels; ++i) {
        DelaySequence(temp.data(), temp.size(), delays[i], input.channel(i));
    }

    auto tdoa = std::make_unique<TDOA>(SampleRate, Channels, SamplesPerChannel);
    tdoa->setMargin(SamplesPerChannel / 2);

    const auto& results = tdoa->process(input);
    for (auto i = 0; i < Channels; ++i) {
        EXPECT_NEAR(results[i] * SampleRate, delays[i], 0.01);
    }
}
