#ifndef SMARTCORE_DELAY_HPP
#define SMARTCORE_DELAY_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Delay {
    public:
        /**
         * @brief Builds a delay block with the given configuration
         * @param sample_rate Sampling frequency in Hz
         * @param delay Delay in milliseconds.
         */
        Delay(std::int32_t sample_rate, std::int32_t delay);

        /**
         * @brief Default destructor
         */
        ~Delay();

        /**
         * @brief Performs a delay in the input buffer and stores the result in the
         * output one.
         * @param input Input buffer storing the audio samples.
         * @param output Output buffer storing the results of the beam-forming.
         */
        void process(const AudioBuffer& input, AudioBuffer& output);
    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };
}

#endif //SMARTCORE_DELAY_HPP
