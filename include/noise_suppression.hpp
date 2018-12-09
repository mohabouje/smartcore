#ifndef SMARTCORE_NOISE_SUPPRESSION_HPP
#define SMARTCORE_NOISE_SUPPRESSION_HPP

#include <audio_buffer.hpp>
#include <vector>
#include <memory>

namespace score {

    class NoiseSuppression {
    public:

        /**
         * @brief Represents the aggressiveness of the noise suppression method
         */
        enum Policy {
            Mild = 0,
            Medium,
            Aggressive
        };

        /**
         * @brief Creates and initializes a Noise Suppression filter.
         * @param sample_rate Sampling frequency in Hz.
         * @param channels Number of channels
         * @param policy Aggressiveness of the noise suppression method.
         * @note Supported sample rates: 8KHz or 16KHz.
         * @throws std::bad_alloc in case of a memory allocation error.
         */
        NoiseSuppression(std::size_t channels,  float sample_rate, Policy policy);

        /**
         * @brief Default destructor.
         */
        ~NoiseSuppression();

        /**
         * @brief Re-initializes the Noise Suppression filter, clearing all state.
         */
        void reset();

        /**
         * @brief Returns a pointer to the noise estimate per frequency bin.
         * @return Vector storing he noise estimate per frequency bin.
         */
        const std::vector<float>& estimatedNoise() const;

        /**
         * @brief  Returns the number of frequency bins
         * @return Number of frequency bins.
         */
        std::size_t numberFrequencyBins() const;

        /**
         * @brief Returns the internally used prior speech probability of the current frame.
         * @return Prior speech probability in interval [0.0, 1.0].
         */
        float speechProbability() const;

        /**
         * @brief Sets the aggressiveness of the noise suppression method
         * @param policy Aggressiveness of the noise suppression method
         */
        void setPolicy(Policy policy);

        /**
         * @brief Perform a Noise-Suppression filter in an audio frame.
         * @note The input and output signals should always be 10ms (80 or 160 samples).
         * @param input Vector storing the input audio samples.
         * @param output Vector storing the output audio samples.
         */
        void process(const AudioBuffer& input, AudioBuffer& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };
}


#endif //SMARTCORE_NOISE_SUPPRESSION_HPP
