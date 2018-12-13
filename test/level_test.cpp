#include <gtest/gtest.h>
#include <level.hpp>
#include <encoder.hpp>

using namespace score;

void RunBitexactnessTest(std::int32_t sample_rate_hz, std::int8_t channels, float rms_reference) {
    constexpr auto NumberFramesToProcess = 1000;
    auto level_estimator = std::make_unique<LevelEstimator>(channels);
}

TEST(LevelEstimatorTest, Mono8kHz) {
    constexpr auto RmsReference = 31.f;
    RunBitexactnessTest(8000, 1, RmsReference);
}

TEST(LevelEstimatorTest, Mono16kHz) {
    constexpr auto RmsReference = 31.f;
    RunBitexactnessTest(16000, 1, RmsReference);
}

TEST(LevelEstimatorTest, Mono32kHz) {
    constexpr auto RmsReference = 31.f;
    RunBitexactnessTest(32000, 1, RmsReference);
}

TEST(LevelEstimatorTest, Mono48kHz) {
    constexpr auto RmsReference = 31.f;
    RunBitexactnessTest(48000, 1, RmsReference);
}

TEST(LevelEstimatorTest, Stereo16kHz) {
    constexpr auto RmsReference = 30.f;
    RunBitexactnessTest(16000, 2, RmsReference);
}
