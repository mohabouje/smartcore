#ifndef SMARTCORE_DOA_H
#define SMARTCORE_DOA_H

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class DOA {
    public:

        /**
         * @brief Build a block to compute the Direction of Arrival of an array of microphones.
         * @param sample_rate Sampling rate in Hz.
         * @param microphone_distances Distances between microphones.
         * @param sound_speed Sound of the speed in m/sec.
         */
        explicit DOA(std::int32_t sample_rate, std::uint8_t num_microphones,
                float microphone_distances = 0.08127, float sound_speed = 343.2f);

        /**
         * @brief Default destructor
         */
        ~DOA();

        /**
         * @brief Returns the current sample rate in Hz.
         * @return Sample rate in Hz.
         */
        float sampleRate() const;

        /**
         * @brief Sets the sample rate of the input microphones.
         * @param sample_rate Sample rate in Hz.
         */
        void setSampleRate(std::int32_t sample_rate);

        /**
         * @brief Returns the speed of the sound in m/secs
         * @return Speed in m/sec
         */
        float soundSpeed() const;

        /**
         * @brief Sets the current speed in m/secs
         * @param Speed in m/sec
         */
        void setSoundSpeed(float speed);

        /**
         * @brief Sets the group of microphones, given by pair of indexes
         * @param microphone_groups Group of microphones
         */
        void setGroupMicrophones(const std::vector<std::pair<std::size_t, std::size_t>>& microphone_groups);

        /**
         * @brief Returns the group of microphones
         * @return Group of microphones in pair of index.
         */
        const std::vector<std::pair<std::size_t, std::size_t>>& groupMicrophones() const;

        /**
         * @brief Computes the direction of arrival of the different microphones
         *
         * @param input Vector of arrays storing the input audio samples.
         * @returns The direction of arrival.
         */
        float process(const AudioBuffer& microphone_inputs);

        /**
         * @brief Re-initializes the block, clearing all state.
         */
        void reset();

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };

}

#endif //SMARTCORE_DOA_H
