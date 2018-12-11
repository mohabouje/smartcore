#ifndef SMARTCORE_LOW_CUT_FILTER_HPP
#define SMARTCORE_LOW_CUT_FILTER_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class LowCutFilter {
    public:
        /**
         * Creates a LowCutFilter that removes low frequencies from the audio frame.
         * @param sample_rate Sampling rate in Hz.
         * @param channels Number of channels.
         */
        LowCutFilter(std::int32_t sample_rate, std::int8_t channels);

        /**
         * @brief Default destructor
         */
        ~LowCutFilter();

        /**
         * @brief Perform a Low-Cut filter in an audio frame.
         * @param input Vector storing the input audio samples.
         * @param output Vector storing the output audio samples.
         */
        void process(const AudioBuffer& input, AudioBuffer& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };
}


#endif //SMARTCORE_LOW_CUT_FILTER_HPP
