#ifndef SMARTCORE_SMOOTH_HPP
#define SMARTCORE_SMOOTH_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Smooth {
    public:
        /**
         * @brief Creates a smooth filter
         * @param channels Number of channels
         * @param window_size Number of samples in the moving window
         */
        Smooth(std::int8_t channels, std::size_t window_size);

        /**
         * @brief Default destructor
         */
        ~Smooth();

        /**
         * @brief Sets the length of the moving window
         * @param window_size Number of samples in the moving window
         */
        void setWindowSize(std::size_t window_size);

        /**
         * @brief Returns the length of the moving window
         * @return Length of the moving window
         */
        std::size_t windowSize() const;

        /**
         * @brief Resets the internal states
         */
        void reset();

        /**
         * @brief Filter the audio frame.
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

#endif //SMARTCORE_SMOOTH_HPP
