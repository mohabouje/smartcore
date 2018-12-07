#ifndef SMARTCORE_RESAMPLE_H
#define SMARTCORE_RESAMPLE_H

#include <memory>
#include <vector>

namespace score {

    class ReSampler {
    public:
        /**
         * @brief The Quality enum describes the different Re-Sampler operation modes
         * in terms of output quality.
         */
        enum Quality {
            Low = 0,
            High
        };

        /**
         * @brief Creates a re sampler with the given configuration
         * @param quality  Quality of the re-sampling process.
         * @param factor   Re-sampling factor (output sample rate / input sample rate)
         */
        ReSampler(Quality quality, float factor);

        /**
         * @brief Default destructor
         */
        ~ReSampler();

        /**
         * @brief Re-initializes the re-sampler, clearing all state
         */
        void reset();

        /**
         * @brief Performs a Re-Sampling filter in an audio frame.
         *
         * @param input Array storing the input audio samples.
         * @param output Array storing the output audio samples.
         * @returns The size of the output array.
         */
        int process(const float* input, float* output, std::size_t size);

        /**
         * @brief Performs a Re-Sampling filter in an audio frame.
         *
         * @param input Vector storing the input audio samples.
         * @param output Vector storing the output audio samples.
         * @returns The size of the output vector.
         */
        int process(const std::vector<float>& input, std::vector<float>& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };

}

#endif //SMARTCORE_RESAMPLE_H
