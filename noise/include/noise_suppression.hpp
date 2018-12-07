#ifndef NOISE_SUPPRESSION_H
#define NOISE_SUPPRESSION_H

#include <vector>
#include <memory>

namespace score {

    class NoiseSuppression {
    public:

        /**
         * @brief Creates and initializes a Noise Suppression filter.
         * @throws std::bad_alloc in case of a memory allocation error.
         */
        NoiseSuppression();

        /**
         * @brief Default destructor.
         */
        ~NoiseSuppression();

        /**
         * @brief Re-initializes the Noise Suppression filter, clearing all state.
         */
        void reset();

        /**
         * @brief Perform a Noise-Suppression filter in an audio frame.
         *
         * An audio `frame` is an array of length 480 at 48 kHz as sample rate.
         *
         * @note The length of the audio frame should be 480 samples and
         * the sample rate 48 kHz.
         *
         * @param input Array storing the input audio samples.
         * @param output Array storing the output audio samples.
         * @throws std::invalid_argument if the length of the frame is invalid.
         */
        void process(const float* input, float* output);

        /**
         * @brief Perform a Noise-Suppression filter in an audio frame.
         *
         * An audio `frame` is an array of length 480 at 48 kHz.
         *
         * @note The length of the audio frame should be 480 samples and
         * the sample rate 48 kHz.
         *
         * @param input Vector storing the input audio samples.
         * @param output Vector storing the output audio samples.
         * @throws std::invalid_argument if the length of the frame is invalid.
         */
        void process(const std::vector<float>& input, std::vector<float>& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };
}

#endif // NOISE_SUPPRESSION_H
