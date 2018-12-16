#include "level.hpp"
#include <edsp/feature/temporal/rms.hpp>
#include <edsp/converter/pow2db.hpp>
#include <numeric>
#include <level.hpp>

using namespace score;

struct LevelEstimator::Pimpl {

    explicit Pimpl(std::int8_t channels) : levels_(channels, 0) {

    }

    const Vector<float> & process(const score::AudioBuffer &input) {
        if (input.channels() != levels_.size()) {
            throw std::invalid_argument("Expected an input frame with "
                                        + std::to_string(levels_.size()) + " channels.");
        }

        const auto channels = input.channels();
        const auto frames = input.framesPerChannel();
        for (auto i = 0ul; i < channels; ++i) {
            const auto rms = edsp::feature::rms(input.channel(i),
                                                input.channel(i) + input.framesPerChannel());
            levels_[i] = edsp::pow2db(rms);
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
