#ifndef SMARTCORE_RNN_DeepVAD_HPP
#define SMARTCORE_RNN_DeepVAD_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class DeepVAD {
    public:

        /**
         * @brief Creates and initializes a DeepVAD instance with the given sample rate.
         * @throws std::bad_alloc in case of a memory allocation error.
         */
        DeepVAD();

        /**
         * @brief Default destructor.
         */
        ~DeepVAD();

        /**
         * @brief Re-initializes the DeepVAD, clearing all state and resetting mode
         *  and sample rate to defaults.
         */
        void reset();

        /**
         * @brief Returns the internally used prior speech probability of the current frame.
         * @return Prior speech probability in interval [0.0, 1.0].
         */
        float speechProbability() const;

        /**
         * @brief Calculates a DeepVAD decision for an audio frame.
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
#endif //SMARTCORE_RNN_DeepVAD_HPP
