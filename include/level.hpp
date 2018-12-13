#ifndef SMARTCORE_LEVEL_ESTIMATOR_HPP
#define SMARTCORE_LEVEL_ESTIMATOR_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class LevelEstimator {
    public:
        /**
         * @brief Creates a multi-channel level estimator
         * @param channels Number of channels
         */
        explicit LevelEstimator(std::int8_t channels);

        /**
         * @brief Default destructor
         */
        ~LevelEstimator();

        /**
         * @brief Computes the average level (rms) of the input frame.
         * @param input Vector storing the input audio samples.
         * @return Vector of the levels at each channel
         */
        const Vector<float> & process(const AudioBuffer& input);

        /**
         * @brief Returns the computed level for each channel
         * @return Vector of the levels at each channel
         */
        const Vector<float>& levels() const;

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };
}

#endif //SMARTCORE_LEVEL_ESTIMATOR_HPP
