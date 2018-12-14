#include <audio_buffer.hpp>

#include <gtest/gtest.h>
#include <algorithm>
#include <numeric>

using namespace score;


constexpr std::int32_t SampleRate = 8000;
constexpr size_t NumberFrames = 480u;
constexpr size_t Stereo = 2u;
constexpr size_t Mono = 1u;


TEST(TestingAudioBuffer, CreateDefault) {
    AudioBuffer buffer(SampleRate, Mono, NumberFrames);
    EXPECT_EQ(buffer.sampleRate(), SampleRate);
    EXPECT_EQ(buffer.channels(), Mono);
    EXPECT_EQ(buffer.framesPerChannel(), NumberFrames);
    EXPECT_EQ(buffer.size(), Mono * NumberFrames);
}

TEST(IFTestingAudioBuffer, CreateFromRawMono) {
    std::array<float, NumberFrames> temporal{}, restored{};
    std::iota(std::begin(temporal), std::end(temporal), 0);

    AudioBuffer buffer(SampleRate, Mono, NumberFrames, temporal.data());
    EXPECT_TRUE(std::equal(std::begin(temporal), std::end(temporal), buffer.channel(0)));

    buffer.toInterleave(restored.data());
    EXPECT_TRUE(std::equal(std::begin(temporal), std::end(temporal), restored.data()));
}


TEST(TestInputAudioFile, CreateFromRawStereo) {
    std::array<float, Stereo * NumberFrames> temporal{}, restored{};
    auto* ptr = temporal.data();
    for (auto i = 0ul; i < NumberFrames; ++i) {
        for (auto j = 0ul; j < Stereo; ++j) {
            *ptr++ = static_cast<short>(i);
        }
    }

    AudioBuffer buffer(SampleRate, Stereo, NumberFrames, temporal.data());
    EXPECT_EQ(buffer.channels(), Stereo);
    EXPECT_EQ(buffer.framesPerChannel(), NumberFrames);
    EXPECT_EQ(buffer.size(), Stereo * NumberFrames);

    std::array<float, NumberFrames> single{};
    std::iota(std::begin(single), std::end(single), 0);
    for (auto i = 0ul; i < Stereo; ++i) {
        EXPECT_TRUE(std::equal(std::begin(single), std::end(single), buffer.channel(i)));
    }

    buffer.toInterleave(restored.data());
    EXPECT_TRUE(std::equal(std::begin(temporal), std::end(temporal), std::begin(restored)));
}

TEST(TestInputAudioFile, UpdateFromRawData) {
    std::array<float, Stereo * NumberFrames> temporal{}, restored{};
    auto* ptr = temporal.data();
    for (auto i = 0ul; i < NumberFrames; ++i) {
        for (auto j = 0ul; j < Stereo; ++j) {
            *ptr++ = static_cast<short>(i);
        }
    }

    AudioBuffer buffer;
    buffer.fromInterleave(Stereo, NumberFrames, temporal.data());

    EXPECT_EQ(buffer.channels(), Stereo);
    EXPECT_EQ(buffer.framesPerChannel(), NumberFrames);
    EXPECT_EQ(buffer.size(), Stereo * NumberFrames);

    std::array<float, NumberFrames> single{};
    std::iota(std::begin(single), std::end(single), 0);
    for (auto i = 0ul; i < Stereo; ++i) {
        EXPECT_TRUE(std::equal(std::begin(single), std::end(single), buffer.channel(i)));
    }

    buffer.toInterleave(restored.data());
    EXPECT_TRUE(std::equal(std::begin(temporal), std::end(temporal), std::begin(restored)));
}
