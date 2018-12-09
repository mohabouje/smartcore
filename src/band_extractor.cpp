
#include "band_extractor.hpp"

#include <cstring>
#include <common_audio/signal_processing/splitting_filter.c>
#include <band_extractor.hpp>


using namespace score;

struct TwoBandsStates {
    TwoBandsStates() {
        reset();
    }

    void reset() {
        memset(analysis_state1, 0, sizeof(analysis_state1));
        memset(analysis_state2, 0, sizeof(analysis_state2));
        memset(synthesis_state1, 0, sizeof(synthesis_state1));
        memset(synthesis_state2, 0, sizeof(synthesis_state2));
    }

    static const int kStateSize = 6;
    int analysis_state1[kStateSize];
    int analysis_state2[kStateSize];
    int synthesis_state1[kStateSize];
    int synthesis_state2[kStateSize];
};


struct BandExtractor::Pimpl {
    
    static constexpr std::size_t NumBands = 2ul;
    
    Pimpl() = default;
    ~Pimpl() = default;
    
    void process(const std::vector<std::int16_t>& input, std::vector<std::vector<std::int16_t>>& output) {
        output.resize(NumBands);

        const auto number_frames = input.size();
        for (auto& band : output) {
            output.resize(number_frames / NumBands);
        }

        WebRtcSpl_AnalysisQMF(input.data(), input.size(),
                              output[0].data(),
                              output[0].data(),
                              two_bands_state_.analysis_state1,
                              two_bands_state_.analysis_state2);
        
    }
    
    void synthesis(const std::vector<std::vector<std::int16_t>>& bands, std::vector<std::int16_t>& output) {
        if (bands.size() != NumBands) {
            throw std::runtime_error("Expected " + std::to_string(NumBands) + " number of bands");
        }    
        
        const auto number_frames = bands.front().size() * NumBands;
        output.resize(number_frames);

        WebRtcSpl_SynthesisQMF(
                bands[0].data(),
                bands[1].data(), 
                bands[0].size(),
                output.data(), 
                two_bands_state_.synthesis_state1,
                two_bands_state_.synthesis_state2);
        
    }

    void reset() {
        two_bands_state_.reset();
    }
    
private:
    TwoBandsStates two_bands_state_;
};

void BandExtractor::process(const std::vector<std::int16_t> &input, std::vector<std::vector<std::int16_t>> &output) {
    pimpl_->process(input, output);
}

void BandExtractor::synthesis(const std::vector<std::vector<std::int16_t>> &bands, std::vector<std::int16_t>& output) {
    pimpl_->synthesis(bands, output);
}

void BandExtractor::reset() {
    pimpl_->reset();
}

void BandExtractor::process(const std::vector<float> &input, std::vector<std::vector<float>> &output) {

}

void BandExtractor::synthesis(const std::vector<std::vector<float>> &bands, std::vector<float> &output) {

}

BandExtractor::BandExtractor() = default;
