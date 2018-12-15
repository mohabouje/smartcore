#ifndef SMARTCORE_ONSET_HPP
#define SMARTCORE_ONSET_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Onset {
    public:
        enum Method {
            Energy,
            HighFrequencyContent,
            ComplexDomain,
            PhaseDomain,
            WeightedPhase,
            SpectralDifference,
            KullbackLiebler,
            ModifiedKullbakLiebler,
            SpectralFlux
        };

        /**
         * @brief Creates a pitch tracker with the given configuration
         * @param method Pitch detection algorithm
         * @param frame_size Size of the input buffer to analyze
         */
        Onset(std::int32_t sample_rate,  Method method, std::size_t frame_size);

        /**
         * @brief Creates a pitch tracker with the given configuration
         * @param method Pitch detection algorithm
         * @param frame_size Size of the input buffer to analyze
         * @param hope_size Step size between two consecutive analysis instance
         */
        Onset(std::int32_t sample_rate, Method method, std::size_t frame_size, std::size_t hope_size);

        /**
         * @brief Default destructor
         */
        ~Onset();

        /**
         * @brief Set the onset detection adaptive whitening
         * @param state If true, enables the onset detection adaptive whitening
         */
        void enableAdaptiveWhitening(bool state);

        /**
         * @brief Check if the onset detection adaptive whitening is enabled
         * @return True if the adaptive whitening is enabled, false otherwise.
         */
        bool isAdaptiveWhiteningEnabled() const;

        /**
         * @brief Returns the logarithmic compression factor
         * @return Logarithmic compression factor
         */
        float logCompression() const;

        /**
         * @brief Sets the logarithmic compression factor
         * @param log_compression Logarithmic compression factor. 0, if disabled.
         */
        void setLogCompression(float log_compression);

        /**
         * @brief Sets the silence level
         * @param silence_level_db Silence level in dB.
         */
        void setSilenceLevel(float silence_level_db);

        /**
         * @brief Returns the silence level
         * @return Silence level in dB.
         */
        float silenceLevel() const;

        /**
         * @brief Returns the current value of the descriptor
         * @return Current value of the descriptor
         */
        float descriptor() const;

        /**
         * @brief Returns the current value of the thresholded descriptor
         * @return Current value of the thresholded descriptor
         */
        float thresholdedDescriptor() const;

        /**
         * @brief Sets the peak-picking threshold
         * @param threshold Peak-picking threshold
         */
        void setThreshold(float threshold);

        /**
         * @brief Returns the peak-picking threshold
         * @return Peak-picking threshold
         */
        float threshold() const;

        /**
         * @brief Set the minimum inter onset interval in milliseconds.
         * @param minioi_msecs Minimum interval between two consecutive onsets.
         */
        void setMinimumInterOnset(float minioi_msecs);

        /**
         * @brief Returns the minimum inter onset interval.
         * @return The minimum inter onset interval in milliseconds.
         */
        float minimumInterOnset() const;

        /**
         * @brief Sets the constant system delay to take back from detection time
         * @param delay_msecs delay Delay time in milliseconds.
         */
        void setDelay(float delay_msecs);

        /**
         * @brief Returns the constant system delay to take back from detection time
         * @return Delay time in milliseconds
         */
        float delay() const;

        /**
         * @brief Reset the onset detector to the default configuration
         */
         void reset();

        /**
         * @brief Onset detection in the given audio frame.
         *
         * @param samples Vector with the audio samples.
         * @return Pair holding the state of the detection and the moment where the onset was detected in msecs.
         */
        std::pair<bool, float> process(const AudioBuffer& input);


    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };

}

#endif //SMARTCORE_ONSET_HPP
