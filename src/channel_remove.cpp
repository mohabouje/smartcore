#include "channel_remove.hpp"
#include <unordered_set>
#include <channel_remove.hpp>


using namespace score;

struct ChannelRemove::Pimpl {

    explicit Pimpl(std::uint8_t channels) :
        channels_(channels) {

    }


    void ignore(std::uint8_t index) {
        if (index < channels_) {
            ignored_.insert(index);
        }
    }

    void process(const AudioBuffer& input, AudioBuffer& output) {
        if (channels_ != input.channels()) {
            throw std::invalid_argument("The block is configured to work with "
                                        + std::to_string(channels_) + " channels.");
        }

        output.setSampleRate(input.sampleRate());
        output.resize(remaining(), input.framesPerChannel());
        for (auto i = 0ul, inserted = 0ul; i < channels_; ++i) {
            if (ignored_.find(i) == ignored_.end()) {
                std::copy(input.channel(i), input.channel(i) + input.framesPerChannel(), output.channel(inserted++));
            }
        }
    }

    std::size_t remaining() {
        return channels_ - ignored_.size();
    }

    std::uint8_t channels_{};
    std::unordered_set<std::size_t> ignored_{};
};

score::ChannelRemove::ChannelRemove(std::uint8_t channels) :
    pimpl_(std::make_unique<Pimpl>(channels)){

}

void score::ChannelRemove::ignore(std::uint8_t index) {
    pimpl_->ignore(index);
}

void score::ChannelRemove::process(const AudioBuffer &input, AudioBuffer &output) {
    pimpl_->process(input, output);
}

std::size_t score::ChannelRemove::remaining() const {
    return pimpl_->remaining();
}

score::ChannelRemove::~ChannelRemove() {

}
