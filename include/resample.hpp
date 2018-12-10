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
            BestQuality		= 0,
            MediumQuality,
            SincFastest,
            ZeroOrder,
            Linear
        };

        /**
         * @brief Creates a re sampler with the given configuration
         * @param quality  Quality of the re-sampling process.
         * @param ratio   Re-sampling ratio (output sample rate / input sample rate)
         */
        ReSampler(std::size_t channels, Quality quality, float ratio);

        /**
         * Checks if the ratio is valid.
         * @param ratio
         * @return True if the ratio is valid, false otherwise.
         */
        static bool isValidRatio(float ratio);

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
