
#include "band_extractor.hpp"

#include <cstring>
#include <common_audio/signal_processing/splitting_filter.c>

using namespace score;


void BandExtractor::process(const std::int16_t* input, std::size_t input_size, Matrix<int16_t >& output) {

    const auto number_frames = input_size;
    output.resize(NumberBands, number_frames / NumberBands);
    WebRtcSpl_AnalysisQMF(input, input_size,
                          output.row(Band0To8kHz).data(),
                          output.row(Band8To16kHz).data(),
                          two_bands_state_.analysis_state1.data(),
                          two_bands_state_.analysis_state2.data());

}

void BandExtractor::synthesis(const Matrix<int16_t>& bands,  std::int16_t* output) {
    if (bands.rows() != Bands::NumberBands) {
        throw std::runtime_error("Expected " + std::to_string(Bands::NumberBands) + " number of bands");
    }

    WebRtcSpl_SynthesisQMF(
            bands.row(Band0To8kHz).data(),
            bands.row(Band8To16kHz).data(),
            static_cast<size_t>(bands.cols()),
            output,
            two_bands_state_.synthesis_state1.data(),
            two_bands_state_.synthesis_state2.data());

}

void BandExtractor::reset() {
    two_bands_state_.reset();
}



BandExtractor::~BandExtractor() = default;

BandExtractor::BandExtractor() = default;
