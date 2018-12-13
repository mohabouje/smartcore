#ifndef SMARTCORE_GAIN_HPP
#define SMARTCORE_GAIN_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Gain {
    public:
        /**
         * @brief Creates a Gain block with the given configuration.
         * @param clip_samples If true, the samples will be clipped to the sample ranges.
         */
        explicit Gain(float gain);

        /**
         * @brief Default destructor
         */
         ~Gain();

        /**
         * @brief Sets the gain factor
         * @param factor Numeric value representing the gain factor.
         */
        void setGainFactor(float factor);

        /**
         * @brief Returns the applied gain factor
         * @return Gain factor.
         */
        float gainFactor() const;

        /**
         * @brief Applies the gain factor to an audio frame.
         *
         * @param input Buffer storing the input audio samples.
         * @param output Buffer storing the output audio samples.
         */
        void process(const AudioBuffer& input, AudioBuffer& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };


}

#endif //SMARTCORE_GAIN_HPP
