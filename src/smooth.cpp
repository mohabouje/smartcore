#include "smooth.hpp"
#include <edsp/filter/moving_average_filter.hpp>
using namespace score;

struct Smooth::Pimpl {

    Pimpl(std::int8_t channels, std::size_t window_size) :
            window_size_(window_size),
            filters_(channels) {
        for (auto& filter : filters_)
            filter = std::make_unique<edsp::filter::moving_average<float>>(window_size);
    }

    void process(const AudioBuffer& input, AudioBuffer& output) {
        if (input.channels() != filters_.size()) {
            throw std::invalid_argument("Expected an input frame with "
                                        + std::to_string(filters_.size()) + " channels.");
        }

        output.setSampleRate(input.sampleRate());
        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < input.channels(); ++i) {
            filters_[i]->filter(input.channel(i), input.channel(i) + input.framesPerChannel(), output.channel(i));
        }

    }

    void reset() {
        for (auto& filter : filters_) {
            filter.reset();
        }
    }
    
    void setWindowSize(std::size_t window_size) {
        window_size_ = window_size;
        for (auto& filter : filters_) {
            filter.reset();
        }
    }

    std::size_t window_size_;
    std::vector<std::unique_ptr<edsp::filter::moving_average<float>>> filters_;
};

score::Smooth::Smooth(std::int8_t channels, std::size_t window_size) :
    pimpl_(std::make_unique<Pimpl>(channels, window_size)){
 
}

void score::Smooth::process(const AudioBuffer &input, AudioBuffer &output) {
    pimpl_->process(input, output);
}

void score::Smooth::reset() {
    pimpl_->reset();
}

size_t score::Smooth::windowSize() const {
    return pimpl_->window_size_;
}

void score::Smooth::setWindowSize(std::size_t window_size) {
    pimpl_->setWindowSize(window_size);
}

score::Smooth::~Smooth() = default;

