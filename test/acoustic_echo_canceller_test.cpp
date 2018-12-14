#include <acoustic_echo_canceller.hpp>
#include <gtest/gtest.h>
using namespace score;

TEST(EchoCancellationTest, Initialization) {
    constexpr auto InputChannels = 2;
    constexpr auto OutputChannels = 2;
    constexpr auto FrameSize = 0.01 * SampleRates::SampleRate8kHz;
    constexpr auto TailSize = 0.01 * SampleRates::SampleRate8kHz;

}
