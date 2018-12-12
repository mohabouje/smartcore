#ifndef SMARTCORE_AUDIO_BUFFER_HPP
#define SMARTCORE_AUDIO_BUFFER_HPP

#include <types.hpp>

namespace score {

    class AudioBuffer {
    public:

        /**
         * @brief Default destructor
         */
        AudioBuffer();

        /**
         * @brief Creates an empty AudioBuffer with the given sample rate
         * @param sample_rate Sample rate in Hz.
         */
        explicit AudioBuffer(std::int32_t sample_rate);

        /**
         * @brief Creates an AudioBuffer with the given settings.
         * @param sample_rate  Sample rate in Hz.
         * @param channels Number of channels in the audio buffer.
         * @param frames_per_channel Number of samples per buffer.
         */
        AudioBuffer(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_channel);

        /**
         * @brief Creates an AudioBuffer with the given settings and copies the raw data.
         * @param sample_rate  Sample rate in Hz.
         * @param channels Number of channels in the audio buffer.
         * @param frames_per_channel Number of samples per buffer.
         * @param raw Array of raw data holding the audio samples.
         */
        AudioBuffer(std::int32_t sample_rate, std::int8_t channels, std::size_t frames_per_channel, const int16_t* raw);

        /**
         * @brief Set the buffer sampling rate.
         * @param sample_rate Sampling rate in Hz.
         */
        void setSampleRate(std::int32_t sample_rate);

        /**
         * @brief Returns the buffer sampling rate.
         * @return Sample rate in Hz.
         */
        std::int32_t sampleRate() const;

        /**
         * @brief Returns the number of channels in the buffer.
         * @return Number of channels.
         */
        std::int8_t channels() const;

        /**
         * @brief Returns the number of samples per buffer
         * @return Number of samples per buffer
         */
        std::size_t framesPerChannel() const;

        /**
         * @brief Returns the duration of the frame
         * @return Duration of the frame in msecs.
         */
        size_t duration() const;

        /**
         * @brief Resize the internal buffer
         * @param channels Number of channels in the audio buffer.
         * @param frames_per_channel Number of samples per buffer.
         */
        void resize(std::int8_t channels, std::size_t frames_per_channel);

        /**
         * @bried Returns the time when the first sample of the buffer was processed in the sound card.
         *
         * @note In case of recording buffer, the timestamp represents the time when the first sample of
         * the input buffer was captured at the ADC input. In case of playback recording buffer, the timestamp
         * represent the time when the first sample of the output buffer will output the DAC
         *
         * @return Timestamp in seconds
         */
        double timestamp() const;

        /**
         * @brief Sets the time when the first sample of the buffer was processed by the sound card.
         * @param timestamp Timestamp in seconds
         */
        void setTimestamp(double timestamp);

        /**
         * @brief Returns the size of the underlying raw data.
         * @brief Size of the internal buffer.
         */
        std::size_t size() const;

        /**
         * @brief Updates the internal raw data and invalidates all the existing data.
         * @param channels Number of channels in the audio buffer.
         * @param frames_per_channel Number of samples per buffer.
         * @param raw Array of raw data holding the audio samples.
         */
        void updateRaw(std::int8_t channels, std::size_t frames_per_channel, const int16_t* raw);

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        std::int16_t* channel(std::size_t channel);

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        const std::int16_t* channel(std::size_t channel) const;

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        std::int16_t* operator[](std::size_t channel);

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        const std::int16_t* operator[](std::size_t channel) const;

        /**
         * @brief Returns the interleaved raw data of the input buffer
         * The length of the array is equal to channels * framesPerBuffer
         * @return Vector storing the internal raw data-
         */
        const std::int16_t* raw() const;

        /**
         * @brief Returns the interleaved raw data of the input buffer
         * The length of the array is equal to channels * framesPerBuffer
         * @return Vector storing the internal raw data-
         */
        std::int16_t* raw();

    private:
        double timestamp_{};
        std::int32_t sample_rate_{};
        std::int8_t channels_{};
        std::size_t frames_per_buffer_{};
        Matrix<std::int16_t> deinterleaved_data_{};
        Vector<std::int16_t> interleaved_data_{};
    };



}

#endif //SMARTCORE_AUDIO_BUFFER_HPP
