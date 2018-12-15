#ifndef SMARTCORE_BEAMFORMER_HPP
#define SMARTCORE_BEAMFORMER_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Beamformer {
    public:

        enum Method {
            DelayAndSum,
            MVDR,
            GSC
        };

        /**
         * @brief Creates a beamformer with the given configuration
         * @param sample_rate Sampling rate in Hz.
         * @param channels Number of channels
         * @param frames_per_channel Number of frames per channel
         * @param learning_rate Learning rate of the GSC algorithm
         * @param reference Reference microphone for the TDOA estimation.
         */
        Beamformer(std::int32_t sample_rate,
                std::int8_t channels,
                std::size_t frames_per_channel,
                float learning_rate = 0.01,
                std::int8_t reference = 0);

        /**
         * @brief Default destructor
         */
         ~Beamformer();

        /**
         * @brief Set the maximum margin
         * @param delay Maximum margin in samples.
         */
        void setMargin(std::int32_t margin);


        /**
         * @brief Returns the current operation mode (technique)
         * @return Method used to perform the beam-forming.
         */
        Method method() const;

        /**
         * @brief Set the operational mode (technique) of the beamformer
         * @param method Method to use to perform the beam-forming
         */
        void setMethod(Method method);

        /**
         * @brief Sets the number of samples used to compute the spectral properties
         * @param nfft Number of samples of the FFT
         */
        void setFFTSize(std::size_t nfft);

        /**
         * @brief If enabled, the algorithm suppose the signal arrive sensors
         * at the same time
         * @param ignore If true, the effects ot time delays are ignored.
         */
        void ignoreTimeDelays(bool ignore);

        /**
         * @brief Checks if the algorithms are supping the signal arrive sensors
         * at the same time.
         */
        bool isIgnoringTimeDelays() const;

        /**
         * @brief Performs a beam-forming in the input buffer and stores the result in the
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


#endif //SMARTCORE_BEAMFORMER_HPP
