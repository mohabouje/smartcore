#ifndef SMARTCORE_PITCH_HPP
#define SMARTCORE_PITCH_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Pitch {
    public:
        /**
         * The method enum represents a list of the pitch detection
         * methods currently available.
         */
        enum Method {
            SchmittTrigger,
            FastHarmonicCombFilter,
            MultipleCombFilter,
            Yin,
            YinFast,
            YinFFT
        };

        /**
         * @brief Creates a pitch tracker with the given configuration
         * @param method Pitch detection algorithm
         * @param frame_size Size of the input buffer to analyze
         */
        Pitch(std::int32_t sample_rate,  Method method, std::size_t frame_size);

        /**
         * @brief Creates a pitch tracker with the given configuration
         * @param method Pitch detection algorithm
         * @param frame_size Size of the input buffer to analyze
         * @param hope_size Step size between two consecutive analysis instance
         */
        Pitch(std::int32_t sample_rate, Method method, std::size_t frame_size, std::size_t hope_size);

        /**
         * @brief Default destructor
         */
        ~Pitch();

        /**
         * @brief Set the output unit of the estimated pitch
         * @param unit Frequency unit to use in the output
         */
        void setUnit(FrequencyUnit unit);

        /**
         * @brief Returns the output unit of the estimated pitch
         * @return Frequency unit used in the output
         */
        FrequencyUnit unit() const;

        /**
         * @brief Set the tolerance threshold
         * @note Applicable in the Yin and YinFFT methods. Tolerance default is 0.15 for YIN and 0.85 for YIN-FFT
         * @param tolerance Level of tolerance
         */
        void setTolerance(float tolerance);

        /**
         * @brief Returns the tolerance threshold
         * @note Applicable in the Yin and YinFFT methods. Tolerance default is 0.15 for YIN and 0.85 for YIN-FFT
         * @return Level of tolerance
         */
        float tolerance() const;

        /**
         * @brief Set the silence threshold
         * @param silence Level threshold under witch pitch should be ignored, in dB.
         */
        void setSilenceLevel(float silence_level_db);

        /**
         * @brief Returns the silence level under witch pitch should be ignored.
         * @return Silence level in dB.
         */
        float silenceLevel() const;

        /**
         * @brief Estimates the pitch of the audio frame.
         *
         * @param samples Vector with the audio samples.
         * @return Pair holding the estimated pitch and the current confidence of the pitch algorithm.
         */
        std::pair<float, float> process(const AudioBuffer& input);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };


}

#endif //SMARTCORE_PITCH_HPP
