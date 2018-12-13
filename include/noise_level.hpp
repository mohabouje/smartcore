#ifndef SMARTCORE_NOISE_LEVEL_HPP
#define SMARTCORE_NOISE_LEVEL_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class NoiseLevel {
    public:
        /**
         * @brief Builds a noise level estimator that works with frames with the given configuration.
         * @param sample_rate Sampling rate in Hz.
         * @param frame_length Number of samples per frames (in each channel)
         */
        NoiseLevel(std::int32_t sample_rate, std::size_t frame_length);

        /**
         * @brief Default destructor
         */
        ~NoiseLevel();

        /**
         * @brief Estimates the average noise level (rms) in the audio frame.
         * @return Vector of the levels at each channel
         */
        float process(const AudioBuffer& input);

        /**
         * @brief Returns the average noise level of the processed frame.
         * @return Average noise level in rms.
         */
        float noiseLevel() const;

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };


}


#endif //SMARTCORE_NOISE_LEVEL_HPP
