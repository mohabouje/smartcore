#ifndef SMARTCORE_DEREVERB_HPP
#define SMARTCORE_DEREVERB_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class DeReverberation {

        /**
         * @brief Creates a de-reverberation block
         * @param sample_rate Sampling rate in Hz
         * @param channels Number of channels
         * @param frames_per_buffer Number of samples to process at one time (should correspond to 10-20 ms)
         */
        DeReverberation(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_buffer);

        /**
         * @brief Default destructor
         */
        ~DeReverberation();

        /**
         * @brief Returns the level of the reverberation
         * @return Level of reverberation
         */
        int level() const;

        /**
         * @brief Sets the level of reverberation
         * @param level_db Level of reverberation
         */
        void setLevel(int level_db);

        /**
         * @brief Returns the reverberation decay.
         * @note The reverberation decay is the time required for the reflections (reverberation) to die away
         * (run out of energy).
         * @return Reverberation decay in seconds.
         */
        int decay() const;

        /**
         * @brief Sets the reverberation decay,
         * @param decay Reverberation decay in seconds.
         */
        void setDecay(int decay);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };
}

#endif //SMARTCORE_DEREVERB_HPP
