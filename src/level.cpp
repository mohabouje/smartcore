#include "level.hpp"
#include <numeric>
#include <level.hpp>

using namespace score;

struct LevelEstimator::Pimpl {



    explicit Pimpl(std::int8_t channels) : levels_(channels, 0) {

    }

    int computeRms(float mean_square) {
        static constexpr int kMinLevelDb = 127;
        static constexpr float kMaxSquaredLevel = 32768 * 32768;
        static constexpr float kMinLevel = 1.995262314968883e-13f;

        if (mean_square <= kMinLevel * kMaxSquaredLevel) {
            // Very faint; simply return the minimum value.
            return kMinLevelDb;
        }
        // Normalize by the max level.
        const float mean_square_norm = mean_square / kMaxSquaredLevel;
        // 20log_10(x^0.5) = 10log_10(x)
        const float rms = 10.f * log10(mean_square_norm);
        // Return the negated value.
        return static_cast<int>(-rms + 0.5f);
    }

    const Vector<float> & process(const score::AudioBuffer &input) {
        if (input.channels() != levels_.size()) {
            throw std::invalid_argument("Expected an input frame with "
                                        + std::to_string(levels_.size()) + " channels.");
        }

        const auto channels = input.channels();
        const auto frames = input.framesPerChannel();
        for (auto i = 0ul; i < channels; ++i) {
            const auto* data = input.channel(i);
            const auto sum_square = std::accumulate(data, data + frames, 0.f,
                                    [](float a, int16_t b) { return a + b * b; });
            levels_[i] = computeRms(sum_square / frames);

        }
        return levels_;
    }

    Vector<float> levels_;
};

score::LevelEstimator::LevelEstimator(std::int8_t channels) :
    pimpl_(std::make_unique<Pimpl>(channels)) {

}

score::LevelEstimator::~LevelEstimator() = default;

const Vector<float> & score::LevelEstimator::process(const score::AudioBuffer &input) {
    return pimpl_->process(input);
}

const Vector<float> &score::LevelEstimator::levels() const {
    return pimpl_->levels_;
}
