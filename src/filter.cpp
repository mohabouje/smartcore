#include "filter.hpp"

#include <edsp/filter.hpp>

using namespace score;
using ButterworthDesigner = edsp::filter::designer<float, edsp::filter::designer_type::Butterworth, 10>;
using ChebyshevDesiger = edsp::filter::designer<float, edsp::filter::designer_type::ChebyshevI, 10>;
using InverseChebyshevDesigner = edsp::filter::designer<float, edsp::filter::designer_type::ChebyshevII, 10>;

struct Filter::Pimpl {
    static constexpr auto MaximumOrder = 100;

    Pimpl(std::int32_t sample_rate, std::int8_t channels) :
            sample_rate_(sample_rate), cascades_(channels) {
        
    }

    void design() {

        const auto designed = [&]() {
            switch (designer_type_) {
                case Filter::DesignerType::Butterworth:
                    switch (filter_type_) {
                        case FilterType::LowPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::Butterworth,
                                    edsp::filter::filter_type::LowPass, 2 * MaximumOrder>(order_, sample_rate_, cut_off_);
                        case FilterType::HighPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::Butterworth,
                                    edsp::filter::filter_type::HighPass, 2 * MaximumOrder>(order_, sample_rate_, cut_off_);
                        case FilterType::BandPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::Butterworth,
                                    edsp::filter::filter_type::BandPass, MaximumOrder>(order_, sample_rate_, center_, bandwidth_);
                        case FilterType::BandStop:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::Butterworth,
                                    edsp::filter::filter_type::BandStop, MaximumOrder>(order_, sample_rate_, center_, bandwidth_);
                        case FilterType::LowShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::Butterworth,
                                    edsp::filter::filter_type::LowShelf, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, gain_);
                        case FilterType::HighShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::Butterworth,
                                    edsp::filter::filter_type::HighShelf, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, gain_);
                        case FilterType::BandShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::Butterworth,
                                    edsp::filter::filter_type::BandShelf, MaximumOrder>(order_, sample_rate_,
                                                                                        center_, bandwidth_, gain_);
                    }
                case Filter::DesignerType::ChebyshevI:
                    switch (filter_type_) {
                        case FilterType::LowPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevI,
                                    edsp::filter::filter_type::LowPass, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, ripple_);
                        case FilterType::HighPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevI,
                                    edsp::filter::filter_type::HighPass, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, ripple_);
                        case FilterType::BandPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevI,
                                    edsp::filter::filter_type::BandPass, MaximumOrder>(order_, sample_rate_,
                                                                                       center_, bandwidth_, ripple_);
                        case FilterType::BandStop:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevI,
                                    edsp::filter::filter_type::BandStop, MaximumOrder>(order_, sample_rate_,
                                                                                       center_, bandwidth_, ripple_);
                        case FilterType::LowShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevI,
                                    edsp::filter::filter_type::LowShelf, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, gain_, ripple_);
                        case FilterType::HighShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevI,
                                    edsp::filter::filter_type::HighShelf, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, gain_, ripple_);
                        case FilterType::BandShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevI,
                                    edsp::filter::filter_type::BandShelf, MaximumOrder>(order_, sample_rate_, center_, bandwidth_, gain_, ripple_);
                    }
                case Filter::DesignerType::ChebyshevII:
                    switch (filter_type_) {
                        case FilterType::LowPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevII,
                                    edsp::filter::filter_type::LowPass, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, ripple_);
                        case FilterType::HighPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevII,
                                    edsp::filter::filter_type::HighPass, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, ripple_);
                        case FilterType::BandPass:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevII,
                                    edsp::filter::filter_type::BandPass, MaximumOrder>(order_, sample_rate_,
                                                                                       center_, bandwidth_, ripple_);
                        case FilterType::BandStop:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevII,
                                    edsp::filter::filter_type::BandStop, MaximumOrder>(order_, sample_rate_,
                                                                                       center_, bandwidth_, ripple_);
                        case FilterType::LowShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevII,
                                    edsp::filter::filter_type::LowShelf, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, gain_, ripple_);
                        case FilterType::HighShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevII,
                                    edsp::filter::filter_type::HighShelf, 2 * MaximumOrder>(order_, sample_rate_, cut_off_, gain_, ripple_);
                        case FilterType::BandShelf:
                            return edsp::filter::make_filter<float, edsp::filter::designer_type::ChebyshevII,
                                    edsp::filter::filter_type::BandShelf, MaximumOrder>(order_, sample_rate_,
                                                                                        center_, bandwidth_, gain_, ripple_);
                    }
            }
            
        }();

        for (auto& cascade : cascades_) {
            cascade = designed;
        }
        need_update_ = false;
    }

    void reset() {
        for (auto& cascade : cascades_) {
            cascade.reset();
        }
    }

    void setOrder(std::size_t order) {
        if (order > MaximumOrder) {
            throw std::runtime_error("Maximum order: " + std::to_string(MaximumOrder));
        }

        order_ = order;
        need_update_ = true;
    }
    
    void process(const AudioBuffer& input, AudioBuffer& output) {
        if (input.channels() != cascades_.size()) {
            throw std::invalid_argument("Expected an input frame with "
                                        + std::to_string(cascades_.size()) + " channels.");
        }

        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Expected an input frame at " + std::to_string(sample_rate_) + " Hz.");
        }

        if (need_update_) {
            design();
        }

        output.setSampleRate(input.sampleRate());
        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < input.channels(); ++i) {
            cascades_[i].filter(input.channel(i), input.channel(i) + input.framesPerChannel(), output.channel(i));
        }

    }

    float cut_off_{500};
    float sample_rate_;
    float center_;
    float bandwidth_;
    float ripple_;
    float gain_;
    bool need_update_{true};
    FilterType filter_type_{Filter::FilterType::LowPass};
    DesignerType designer_type_{Filter::DesignerType::Butterworth};
    std::size_t order_{4};
    std::vector<edsp::filter::biquad_cascade<float, MaximumOrder>> cascades_{};
};

Filter::DesignerType score::Filter::designer() const {
    return pimpl_->designer_type_;
}

void score::Filter::setDesigner(Filter::DesignerType designer) {
    pimpl_->designer_type_ = designer;
    pimpl_->need_update_ = true;
}

Filter::FilterType score::Filter::type() const {
    return pimpl_->filter_type_;
}

void score::Filter::setType(Filter::FilterType type) {
    pimpl_->filter_type_ = type;
    pimpl_->need_update_ = true;
}

std::size_t score::Filter::order() const {
    return pimpl_->order_;
}

void score::Filter::setOrder(std::size_t order) {
    pimpl_->setOrder(order);
}

void score::Filter::setBandwidth(float band) {
    pimpl_->bandwidth_ = band;
    pimpl_->need_update_ = true;
}

void score::Filter::setCenterFrequency(float center_frequency) {
    pimpl_->center_ = center_frequency;
    pimpl_->need_update_ = true;
}

void score::Filter::setGain(float gain_db) {
    pimpl_->gain_ = gain_db;
    pimpl_->need_update_ = true;
}

void score::Filter::setRipple(float ripple_db) {
    pimpl_->ripple_ = ripple_db;
    pimpl_->need_update_ = true;
}

void score::Filter::setCutOff(float frequency) {
    pimpl_->cut_off_ = frequency;
    pimpl_->need_update_ = true;
}

float score::Filter::cutOff() const {
    return pimpl_->cut_off_;
}

float score::Filter::ripple() const {
    return pimpl_->ripple_;
}

float score::Filter::gain() const {
    return pimpl_->gain_;
}

float score::Filter::centerFrequency() const {
    return pimpl_->center_;
}

float score::Filter::bandwidth() const {
    return pimpl_->bandwidth_;
}

void score::Filter::process(const AudioBuffer &input, AudioBuffer &output) {
    pimpl_->process(input, output);
}

score::Filter::Filter(std::int32_t sample_rate, std::int8_t channels) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels)) {

}

void score::Filter::reset() {
    pimpl_->reset();
}

score::Filter::~Filter() = default;
