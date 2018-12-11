#ifndef SMARTCORE_RESAMPLE_H
#define SMARTCORE_RESAMPLE_H

#include <audio_buffer.hpp>
#include <memory>
#include <vector>

namespace score {

    class ReSampler {
    public:
        /**
         * @brief The Quality enum describes the different Re-Sampler operation modes
         * in terms of output quality.
         */
        enum class Quality {
            HighQuality		= 10,
            MediumQuality = 5,
            LowQuality = 0,
        };

        /**
         * @brief Creates a re sampler with the given configuration
         * @param channels Number of channels
         * @param quality  Quality of the re-sampling process.
         * @param input_rate Input sampling rate in Hz.
         * @param output_rate Output sampling rate in Hz.
         */
        ReSampler(std::uint8_t channels, std::uint32_t input_rate, std::uint32_t output_rate, Quality quality);

        /**
         * @brief Default destructor
         */
        ~ReSampler();

        /**
         * @brief Re-initializes the re-sampler, clearing all state
         */
        void reset();

        /**
         * @brief Returns the current ratio.
         * @return Current ratio (output sample rate / input sample rate).
         */
        float ratio() const;

        /**
         * @brief Set a new ratio (output sample rate / input sample rate).
         * @throws std::runtime_error If the ratio is invalid.
         * @param ratio Output sample rate / input sample rate).
         */
        void setRatio(float ratio);

        /**
         * @brief Returns the current algorithm quality
         * @return Quality of the algorithm.
         */
        Quality quality() const;

        /**
         * @brief Performs a Re-Sampling filter in an audio frame.
         *
         * @param input Buffer storing the input audio samples.
         * @param output Buffer storing the output audio samples.
         */
        void process(const AudioBuffer& input, AudioBuffer& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };

}

#endif //SMARTCORE_RESAMPLE_H
