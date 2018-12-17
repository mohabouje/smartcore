#ifndef SMARTCORE_CHANNEL_REMOVE_HPP
#define SMARTCORE_CHANNEL_REMOVE_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class ChannelRemove {
    public:

        /**
         * @brief Creates a ChannelRemove block
         * @param channels Number of channels of the input signal
         */
        explicit ChannelRemove(std::uint8_t channels);

        /**
         * @brief Default destructor
         */
        ~ChannelRemove();

        /**
         * @brief Ignores the given channel while processing the data to the output buffer
         * @param index Index of the channel that may be ignored
         */
        void ignore(std::uint8_t index);

        /**
         * @brief Copies the channels that has not been invalidated to the output buffer
         * @param input Input buffer
         * @param output Output buffer
         */
        void process(const AudioBuffer& input, AudioBuffer& output);

        /**
         * @brief Returns the number of channels that has not been invalidated
         * @return
         */
        std::size_t remaining() const;

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };

}

#endif //SMARTCORE_CHANNEL_REMOVE_HPP
