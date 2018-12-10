#ifndef SMARTCORE_RESIDUAL_ECHO_SUPPRESSION_HPP
#define SMARTCORE_RESIDUAL_ECHO_SUPPRESSION_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {


    class ResidualEchoSuppression {
    public:
        /**
         * @brief Creates a new multi-channel echo canceller
         * @param sample_rate Sampling rate in Hz
         * @param channels Number of channels
         * @param frames_per_buffer Number of samples to process at one time (should correspond to 10-20 ms)
         */
        ResidualEchoSuppression(float sample_rate, std::size_t channels, std::size_t frames_per_buffer);

        /**
         * @brief Default destructor
         */
        ~ResidualEchoSuppression();

        /**
         * @brief Sets the maximum attenuation of the residual echo in dB
         * @param attenuation_db Negative number representing the attenuation in dB.
         */
        void setMaximumAttenuation(int attenuation_db);

        /**
         * @brief Returns the maximum attenuation of the residual echo in dB
         * @return Negative number representing the attenuation in dB.
         */
        int maximumAttenuation() const;

        /**
         * @brief Sets the maximum attenuation of the residual echo in dB
         * @param attenuation_db Negative number representing the attenuation in dB.
         */
        void setMaximumAttenuationNearEnd(int attenuation_db);

        /**
         * @brief Returns the maximum attenuation of the residual echo in dB
         * @return Negative number representing the attenuation in dB.
         */
        int maximumAttenuationNearEnd() const;

        /**
         * @brief Performs echo cancellation a frame, based on the audio sent to the speaker (no delay is added
         * to playback in this form)
         *
         * @param recorded Input buffer
         * @param output Output buffer
         */
        void process(const AudioBuffer& recorded, AudioBuffer& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };


}

#endif //SMARTCORE_RESIDUAL_ECHO_SUPPRESSION_HPP
