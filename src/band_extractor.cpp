
#include "band_extractor.hpp"

#include <cstring>
#include <common_audio/signal_processing/splitting_filter.c>

using namespace score;


void BandExtractor::process(const std::vector<std::int16_t>& input, std::vector<std::vector<std::int16_t>>& output) {
    output.resize(Bands::NumberBands);

    const auto number_frames = input.size();
    for (auto& band : output) {
        output.resize(number_frames / Bands::NumberBands);
    }

    WebRtcSpl_AnalysisQMF(input.data(), input.size(),
                          output[0].data(),
                          output[0].data(),
                          two_bands_state_.analysis_state1.data(),
                          two_bands_state_.analysis_state2.data());

}

void BandExtractor::synthesis(const std::vector<std::vector<std::int16_t>>& bands, std::vector<std::int16_t>& output) {
    if (bands.size() != Bands::NumberBands) {
        throw std::runtime_error("Expected " + std::to_string(Bands::NumberBands) + " number of bands");
    }

    const auto number_frames = bands.front().size() * Bands::NumberBands;
    output.resize(number_frames);

    WebRtcSpl_SynthesisQMF(
            bands[0].data(),
            bands[1].data(),
            bands[0].size(),
            output.data(),
            two_bands_state_.synthesis_state1.data(),
            two_bands_state_.synthesis_state2.data());

}

void BandExtractor::reset() {
    two_bands_state_.reset();
}



BandExtractor::~BandExtractor() = default;

BandExtractor::BandExtractor() = default;
