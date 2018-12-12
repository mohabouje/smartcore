#ifndef SMARTCORE_DOWNMIX_HPP
#define SMARTCORE_DOWNMIX_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class DownMix {
    public:
        /**
         * @brief Creates a down-mixing block that works in the given band
         * @param band Band of frequencies used to generate the down mixed frame.
         */
        explicit DownMix(Bands band);

        /**
         * @brief Default destructor
         */
        ~DownMix();

        /**
         * @brief Returns the band of frequencies used to generate the down mixed frame
         * @return Band of operation.
         */
        Bands band() const;

        /**
         * @brief Sets the band of frequencies used to generate the down mixed frame
         * @param band Band of operation.
         */
        void setBand(Bands band);

        /**
         * @brief Performs a Down-Mixing filter in an audio frame.
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

#endif //SMARTCORE_DOWNMIX_HPP
