#ifndef SMARTCORE_TDOA_HPP
#define SMARTCORE_TDOA_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class TDOA {
    public:
        /**
         * Creates a TDOA estimator with the given configuration
         * @param sample_rate Sampling frequency in Hz.
         * @param channels Number of channels
         * @param reference Reference microphone: [0, channels - 1].
         */
        TDOA(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_buffer, std::int8_t reference = 0);

        /**
         * Default destructor
         */
        ~TDOA();

        /**
         * @brief Set the maximum margin
         * @param delay Maximum margin in samples.
         */
        void setMargin(std::int32_t margin);

        /**
         * @brief Estimated the time delay (TOA) in each channel
         * @param input Audio frame containing the audio samples.
         * @return Array holding the estimated time-delay in seconds in each channel
         */
        const Vector<float> process(const AudioBuffer& input);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };


}

#endif //SMARTCORE_TDOA_HPP
