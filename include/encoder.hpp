#ifndef SMARTCORE_ENCODER_HPP
#define SMARTCORE_ENCODER_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Encoder {
    public:
        /**
         * @brief Encodes AudioBuffer in to the given file in  WAV format.
         * @param file Path of the audio file.
         * @param sample_rate Sampling frequency in Hz
         * @param channels Number of channels.
         */
        Encoder(const std::string& file, std::int32_t sample_rate, std::int8_t channels);

        /**
         * @brief Default destructor.
         */
        ~Encoder();

        /**
         * @brief Encodes the AudioBuffer in the audio file.
         *
         * @param input Buffer storing the input audio samples.
         */
        void process(const AudioBuffer& input);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };



}
#endif //SMARTCORE_ENCODER_HPP
