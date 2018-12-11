#ifndef SMARTCORE_ECHO_CANCELLER_HPP
#define SMARTCORE_ECHO_CANCELLER_HPP

#include <audio_buffer.hpp>
#include <vector>

namespace score {

    class AEC {
    public:
        /**
         * @brief Creates a new multi-channel echo canceller
         * @param sample_rate Sampling rate in Hz
         * @param frame_size Number of samples to process at one time (should correspond to 10-20 ms)
         * @param filter_length Number of samples of echo to cancel (should generally correspond to 100-500 ms)
         * @param record_channels Number of microphone channels
         * @param played_channels Number of speaker channels
         *
         * @note The filter length is also known as tail length. The recommended tail length is approximately
         * the third of the room reverberation time. When it comes to echo tail length (filter length), longer is *not*
         * better. Actually, the longer the tail length, the longer it takes for the filter to adapt.
         */
        AEC(std::int32_t sample_rate, std::size_t record_channels,
            std::size_t played_channels, std::size_t frame_size, std::size_t filter_length);

        /**
         * @brief Default destructor
         */
        ~AEC();

        /**
         * @brief Performs echo cancellation a frame, based on the audio sent to the speaker (no delay is added
         * to playback in this form)
         *
         * @param recorded Signal from the microphone (near end + far end echo)
         * @param played Signal played to the speaker (received from far end)
         * @param output Returns near-end signal with echo removed
         *
         * @note The input signal and the echo signal must be small enough because otherwise part of the
         * echo cancellation filter is inefficient. In the worst situations, the delay is such that it is longer than
         * the filter length, in which case, no echo can be cancelled.
         *
         * @note Using a different sound card to do the capture and playback will *not* work. The sources may be generated
         * from the same sound card.
         */
        void process(const AudioBuffer& recorded, const AudioBuffer& played, AudioBuffer& output);


    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };
}

#endif //SMARTCORE_ECHO_CANCELLER_HPP
