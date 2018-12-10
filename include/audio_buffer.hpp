#ifndef SMARTCORE_AUDIO_BUFFER_HPP
#define SMARTCORE_AUDIO_BUFFER_HPP

#include <band_extractor.hpp>
#include <memory>
#include <vector>

namespace score {

    class AudioBuffer {
    public:

        /**
         * @brief Creates an empty AudioBuffer with the given sample rate
         * @param sample_rate Sample rate in Hz.
         */
        explicit AudioBuffer(float sample_rate);

        /**
         * @brief Creates an AudioBuffer with the given settings.
         * @param sample_rate  Sample rate in Hz.
         * @param channels Number of channels in the audio buffer.
         * @param frames_per_channel Number of samples per buffer.
         */
        AudioBuffer(float sample_rate, std::size_t channels, std::size_t frames_per_channel);

        /**
         * @brief Returns the buffer sampling rate.
         * @return Sample rate in Hz.
         */
        float sampleRate() const;

        /**
         * @brief Returns the number of channels in the buffer.
         * @return Number of channels.
         */
        std::size_t channels() const;

        /**
         * @brief Returns the number of samples per buffer
         * @return Number of samples per buffer
         */
        std::size_t framesPerChannel() const;

        /**
         * @brief Returns the number of bands in each channel.
         * @see Bands
         * @return Number of bands in each channel.
         */
        size_t numberBands() const;


        /**
         * @brief Resizes the internal buffer
         * @param channels Number of channels in the audio buffer.
         * @param frames_per_channel Number of samples per buffer.
         */
        void resize(std::size_t channels, std::size_t frames_per_channel);

        /**
         * @brief Updates the audio buffer by merging the different sub-bands
         * @see Bands
         */
        void merge();

        /**
         * @brief Returns a mono signal generated from the low band of each channel in fixed point.
         * @return Down mixed signal in fixed point.
         */
        const std::vector<std::int16_t>& downmix() const;

        /**
         * @brief Returns a mono signal generated from the low band of each channel in floating point.
         * @return Down mixed signal in floating point.
         */
        const std::vector<float>& downmix_f() const;

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        std::vector<std::int16_t>& channel(std::size_t channel);

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        const std::vector<std::int16_t>& channel(std::size_t channel) const;

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        std::vector<float>& channel_f(std::size_t channel);

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        const std::vector<float>& channel_f(std::size_t channel) const;

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        std::vector<std::int16_t >& operator[](std::size_t channel);

        /**
         * @brief Returns the buffer of the given channel.
         * @param channel Desired channel.
         * @return The channel's buffer.
         */
        const std::vector<std::int16_t >& operator[](std::size_t channel) const;

        /**
         * @brief Returns the band's buffer of the given channel.
         * @param channel Desired channel.
         * @param band Desired band.
         * @see band
         * @return The channel's buffer in the given band.
         */
        std::vector<std::int16_t>& band(std::size_t channel, Bands band);

        /**
         * @brief Returns the band's buffer of the given channel.
         * @param channel Desired channel.
         * @param band Desired band.
         * @see band
         * @return The channel's buffer in the given band.
         */
        const std::vector<std::int16_t>& band(std::size_t channel, Bands band) const;

        /**
         * @brief Returns the band's buffer of the given channel.
         * @param channel Desired channel.
         * @param band Desired band.
         * @see band
         * @return The channel's buffer in the given band.
         */
        std::vector<float>& band_f(std::size_t channel, Bands band);

        /**
         * @brief Returns the band's buffer of the given channel.
         * @param channel Desired channel.
         * @param band Desired band.
         * @see band
         * @return The channel's buffer in the given band.
         */
        const std::vector<float>& band_f(std::size_t channel, Bands band) const;

    private:
        /**
         * Invalidates the fixed point configuration flags
         * @param data Flag to update the fixed point audio buffers in each channel.
         * @param bands Flag to update the different fixed point bands buffers in each channel.
         * @param mixed Flag to update the fixed point down mixed signal.
         */
        void invalidate_fixed_data(bool data, bool bands, bool mixed);

        /**
         * Invalidates the floating point configuration flags
         * @param data Flag to update the floating point audio buffers in each channel.
         * @param bands Flag to update the different floating point bands buffers in each channel.
         * @param mixed Flag to update the floating point down mixed signal.
         */
        void invalidate_floating_data(bool data, bool bands, bool mixed);

        /**
         * Ensures that the different fixed point buffers are up-to-date
         * @note This function should called  before each read-only operation.
         * @param update_bands Flag to ensure the correctness of the different bands in each channel.
         * @param update_mixed Flag to ensure the correctness of the down-mixed signal.
         */
        void start_fixed_operation(bool update_bands = false, bool update_mixed = false) const;

        /**
         * Ensures that the different floating point buffers are up-to-date
         * @note This function should called  before each read-only operation.
         * @param update_bands Flag to ensure the correctness of the different bands in each channel.
         * @param update_mixed Flag to ensure the correctness of the down-mixed signal.
         */
        void start_floating_operation(bool update_bands = false, bool update_mixed = false) const;

        /**
         * Ensures that the different fixed point buffers are up-to-date and invalidate the floating point data-
         * @note This function should called before each read-write operation.
         * @param update_bands Flag to ensure the correctness of the different bands in each channel.
         * @param update_mixed Flag to ensure the correctness of the down-mixed signal.
         */
        void start_fixed_operation_and_invalidate(bool update_bands = false, bool update_mixed = false);

        /**
         * Ensures that the different floating point buffers are up-to-date and invalidate the fixed point data-
         * @note This function should called before each read-write operation.
         * @param update_bands Flag to ensure the correctness of the different bands in each channel.
         * @param update_mixed Flag to ensure the correctness of the down-mixed signal.
         */
        void start_floating_operation_and_invalidate(bool update_bands = false, bool update_mixed = false);

        /**
         * Refresh the fixed point data performing a copy-conversion from the floating point buffer.
         * @note This function will be executed only if the fixed point flag has been invalidated.
         */
        void refresh_fixed();

        /**
         * Refresh the floating point data performing a copy-conversion from the fixed point buffer.
         * @note This function will be executed only if the floating point flag has been invalidated.
         */
        void refresh_floating();

        // Helper functions.
        void split_helper();
        void split_helper_f();
        void mixed_helper(Bands band_index);
        void mixed_helper_f(Bands band_index);

    private:
        float sample_rate_{};
        std::size_t channels_{};
        std::size_t frames_{};
        std::vector<std::vector<std::int16_t>> fixed_data_{};
        std::vector<std::vector<float>> floating_data_{};
        std::vector<std::int16_t> fixed_mixed_{};
        std::vector<float> floating_mixed_{};
        std::vector<BandExtractor> band_extractor_{};
        std::vector<std::vector<std::vector<std::int16_t>>> fixed_bands_{};
        std::vector<std::vector<std::vector<float>>> floating_bands_{};
        bool fixed_data_still_valid_{false};
        bool floating_data_still_valid_{false};
        bool fixed_bands_still_valid_{false};
        bool floating_bands_still_valid_{false};
        bool fixed_mixed_still_valid_{false};
        bool floating_mixed_still_valid_{false};
    };



}

#endif //SMARTCORE_AUDIO_BUFFER_HPP
