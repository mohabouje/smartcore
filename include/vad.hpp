#ifndef LIBVAD_VAD_HPP
#define LIBVAD_VAD_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class VAD {
    public:

        /**
         * @brief The Mode enum describes the different VAD operation modes.
         */
        enum Mode {
            Quality = 0,
            LowBitRate,
            Aggressive,
            VeryAggressive
        };

        /**
         * @brief Creates and initializes a VAD instance with the given sample rate.
         * @param sample_rate Sampling rate in Hz.
         * @param mode Operation mode
         * @throws std::bad_alloc in case of a memory allocation error.
         */
        explicit VAD(std::int32_t sample_rate, Mode mode = Mode::Aggressive);

        /**
         * @brief Default destructor.
         */
        ~VAD();

        /**
         * @brief Returns an array with all the supported sample rates in Hz.
         * @return An array storing all the supported sample rates in Hz.
         */
        static std::vector<std::int32_t> SupportedSampleRates();

        /**
         * @brief Returns an array with all the supported frame length in msecs.
         * @return An array storing all the supported frame length in msecs.
         */
        static std::vector<std::int32_t> SupportedFrameDuration();

        /**
         * @brief Sets the input sample rate in Hz for a VAD instance.
         *
         * Valid values are 8000, 16000, 32000 and 48000. The default is 8000Hz.
         * @note Internally all processing will be done 8000 Hz; input data in higher sample rates
         * will just be down sampled first.
         *
         * @param sampleRate Input sample rate in Hz.
         * @throws std::invalid_argument if the passed value is invalid.
         */
        void setSampleRate(std::int32_t sampleRate);

        /**
         * @brief Returns the current input sample rate.
         * @return Input sample rate in Hz.
         */
        std::int32_t sampleRate() const;

        /**
         * @brief Changes the VAD operating ("aggressiveness") mode.
         *
         * A more aggressive (higher mode) VAD is more restrictive in reporting speech.
         * Put in other words the probability of being speech when the VAD returns 1 is
         * increased with increasing mode. As a consequence also the missed detection
         * rate goes up.
         *
         * @param mode VAD operation mode.
         * @see Mode
         */
        void setMode(Mode mode);

        /**
         * @brief Returns the VAD operating ("aggressiveness") mode.
         * @return VAD operation mode.
         */
        Mode mode() const;

        /**
         * @brief Re-initializes the VAD, clearing all state and resetting mode
         *  and sample rate to defaults.
         */
        void reset();

        /**
         * @brief Calculates a VAD decision for an audio frame.
         *
         * An audio `frame` is an array of `length` signed 16-bit samples. Only frames with a
         * length of 10, 20 or 30 ms are supported, so for example at 8 kHz, `length`
         * must be either 80, 160 or 240.
         *
         * @param samples Vector with the audio samples.
         * @throws std::invalid_argument if the length of the frame is invalid.
         * @return True in case of voice activity, false otherwise.
         */
        bool process(const AudioBuffer& input);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };

}


#endif //LIBVAD_VAD_HPP
