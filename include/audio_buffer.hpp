#ifndef SMARTCORE_AUDIO_BUFFER_HPP
#define SMARTCORE_AUDIO_BUFFER_HPP

#include <band_extractor.hpp>
#include <memory>
#include <vector>

namespace score {

    class AudioBuffer {
    public:

        enum Bands {
            Band0To8kHz = 0,
            Band8To16kHz
        };


        explicit AudioBuffer(float sample_rate);
        AudioBuffer(float sample_rate, std::size_t channels, std::size_t frames_per_channel);

        float sampleRate() const;
        std::size_t channels() const;
        std::size_t framesPerChannel() const;
        void resize(std::size_t channels, std::size_t frames_per_channel);

        void merge();

        const std::vector<std::int16_t>& downmix() const;
        const std::vector<float>& downmix_f() const;

        std::vector<std::int16_t>& channel(std::size_t channel);
        const std::vector<std::int16_t>& channel(std::size_t channel) const;

        std::vector<float>& channel_f(std::size_t channel);
        const std::vector<float>& channel_f(std::size_t channel) const;

        std::vector<std::int16_t>& band(std::size_t channel, std::size_t band);
        const std::vector<std::int16_t>& band(std::size_t channel, std::size_t band) const;

        std::vector<float>& band_f(std::size_t channel, std::size_t band);
        const std::vector<float>& band_f(std::size_t channel, std::size_t band) const;

        std::vector<std::int16_t >& operator[](std::size_t channel);
        const std::vector<std::int16_t >& operator[](std::size_t channel) const;

    private:
        std::size_t channels_{};
        std::size_t frames_{};
        float sample_rate_{};
        mutable std::vector<std::vector<std::int16_t>> fixed_data_{};
        mutable std::vector<std::vector<float>> floating_data_{};
        mutable std::vector<std::int16_t> fixed_mixed_{};
        mutable std::vector<float> floating_mixed_{};
        mutable std::vector<BandExtractor> band_extractor_{};
        mutable std::vector<std::vector<std::vector<std::int16_t>>> fixed_bands_{};
        mutable std::vector<std::vector<std::vector<float>>> floating_bands_{};
        mutable bool fixed_data_still_valid_{false};
        mutable bool floating_data_still_valid_{false};
        mutable bool fixed_bands_still_valid_{false};
        mutable bool floating_bands_still_valid_{false};
        mutable bool fixed_mixed_still_valid_{false};
        mutable bool floating_mixed_still_valid_{false};

        void invalidate_fixed_data(bool data, bool bands, bool mixed);
        void invalidate_floating_data(bool data, bool bands, bool mixed);

        void starting_fixed_operation(bool update_bands = false, bool update_mixed = false) const;
        void starting_floating_operation(bool update_bands = false, bool update_mixed = false) const;

        void starting_fixed_operation(bool update_bands = false, bool update_mixed = false);
        void starting_floating_operation(bool update_bands = false, bool update_mixed = false);

        void refresh_fixed() const;
        void refresh_floating() const;

        void split_helper() const;
        void split_helper_f() const;

        void mixed_helper(std::size_t band_index) const;
        void mixed_helper_f(std::size_t band_index) const;

    };



}

#endif //SMARTCORE_AUDIO_BUFFER_HPP
