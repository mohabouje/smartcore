#ifndef SMARTCORE_AUTOMATIC_GAIN_CONTROL_HPP
#define SMARTCORE_AUTOMATIC_GAIN_CONTROL_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class AGC {
    public:

        /**
         * The Mode enums represent the different operational mode of the AGC block.
         */
        enum Mode {

            // Adaptive mode intended for use if an analog volume control is available
            // on the capture device. It will require the user to provide coupling
            // between the OS mixer controls and AGC through the |stream_analog_level()|
            // functions.
            //
            // It consists of an analog gain prescription for the audio device and a
            // digital compression stage.
            AdaptiveAnalog = 0,

            // Adaptive mode intended for situations in which an analog volume control
            // is unavailable. It operates in a similar fashion to the adaptive analog
            // mode, but with scaling instead applied in the digital domain. As with
            // the analog mode, it additionally uses a digital compression stage.
            AdaptiveDigital,

            // Fixed mode which enables only the digital compression stage also used by
            // the two adaptive modes.
            //
            // It is distinguished from the adaptive modes by considering only a
            // short time-window of the input signal. It applies a fixed gain through
            // most of the input level range, and compresses (gradually reduces gain
            // with increasing level) the input signal at higher levels. This mode is
            // preferred on embedded devices where the capture signal level is
            // predictable, so that a known gain can be applied.
            FixedDigital
        };

        /**
         * @brief Creates an AGC with the given configuration
         * @param sample_rate Sampling rate in Hz.
         * @param channels Number of channels.
         * @param mode Operational mode.
         * @param minimum_capture_level Minimum value of an analog sample.
         * @param maximum_capture_level Maximum value of an analog sample.
         */
        AGC(std::int32_t sample_rate, std::int8_t channels, Mode = Mode::AdaptiveDigital,
                int minimum_capture_level = 0, int maximum_capture_level = 255);

        /**
         * @brief Default destructor.
         */
        ~AGC();

        /**
         * @brief Sets the operational mode of the AGC.
         * @see Mode
         * @param mode Operational mode of the AGC.
         */
        void setMode(Mode mode);

        /**
         * @brief Returns the operational mode of the AGC.
         * @return Operational mode of the AGC.
         */
        Mode mode() const;

        /**
         * @brief Sets the target peak (or envelope) of the AGC in dBFs (decibels from digital full-scale)
         * @note Limited to [0, 31].
         * @param level_dbfs The target level in dB below full-scale.
         */
        void setTargetLevel(int level_dbfs);

        /**
         * @brief Returns the target peak (or envelope) of the AGC in dBFs (decibels from digital full-scale).
         * @return The target level in dB below full-scale.
         */
        int targetLevel() const;

        /**
         * Sets the |minimum| and |maximum| analog levels of the audio capture device.
         * @note Must be set if and only if an analog mode is used.
         * @note Limited to [0, 65535].
         * @param min Minimum value of an analog sample.
         * @param max Maximum value of an analog sample.
         */
        void setAnalogLevelRange(int min, int max);

        /**
         * @brief Set the analog level
         * @note This function should be called before processing in Analog mode.
         * @param level Level of the analog target
         */
        void setAnalogLevel(int level);

        /**
         * @brief Returns the minimum and maximum analog levels of the audio capture device
         * @return Pair storing the minimum and maximum levels.
         */
        std::pair<int, int> analogLevelRange() const;

        /**
         * @brief Control the states of the internal limiter.
         * When the limiter is enabled, the compression stage will hard limit the signal to the target level.
         * Otherwise, the signal will be compressed but not limited
         * @param enabled State of the limiter.
         */
        void enableLimiter(bool enabled);

        /**
         * @brief Checks if the internal limiter is enabled.
         * @return True if the limiter is enabled, false otherwise.
         */
        bool isLimiterEnabled() const;

        /**
         * @brief Sets the maximum |gain| the digital compression stage may apply, in dB.
         * A higher number corresponds to greater compression, while a value of 0 will leave the signal uncompressed.
         * @note Limited to [0, 90].
         * @param gain_db Gain applied in the digital compression state in dB.
         */
        void setCompressionGain(int gain_db);

        /**
         * @brief Returns the maximum gain the digital compression state may apply, in dB.
         * @return  Gain applied in the digital compression state in dB.
         */
        int compressionGain() const;

        /**
         * @brief Returns true if the AGC has detected a saturation event (period where the signal reaches digital
         * full-scale) in the current frame and the analog level cannot be reduced.
         * @note This could be used as an indicator to reduce or disable analog mic gain at the audio HAL.
         *
         * @return True if the processed signal is saturated, false otherwise.
         */
        bool isSignalSaturated() const;

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };



}

#endif //SMARTCORE_AUTOMATIC_GAIN_CONTROL_HPP
