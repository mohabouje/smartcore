#ifndef SMARTCORE_BAND_EXTRACTOR_HPP
#define SMARTCORE_BAND_EXTRACTOR_HPP

#include <types.hpp>
#include <memory>
#include <cstring>

namespace score {

    class BandExtractor {
    public:

        /**
         * @brief Default constructor
         */
        BandExtractor();

        /**
         * @brief Default destructor
         */
        ~BandExtractor();

        /**
         * @brief Re-initializes the Noise Suppression filter, clearing all state.
         */
        void reset();

        /**
         * @brief Splits the input buffer in two different bands (low/high).
         * @param input Input buffer.
         * @param output Output buffer storing the different bands.
         */
        void process(const std::int16_t* input, std::size_t input_size, Matrix<int16_t >& output);

        /**
         * @brief Merges the low/high band in to a single audio frame.
         * @param bands Input buffer storing the different bands.
         * @param output Generated audio frame.
         */
        void synthesis(const Matrix<int16_t> &bands, std::int16_t * output);

    private:
        struct BandsStates {
            void reset() {
                std::fill(analysis_state1.begin(), analysis_state1.end(), 0);
                std::fill(analysis_state2.begin(), analysis_state2.end(), 0);
                std::fill(synthesis_state1.begin(), synthesis_state1.end(), 0);
                std::fill(synthesis_state2.begin(), synthesis_state2.end(), 0);
            }

            static constexpr auto StateSize = 6;
            std::array<int, StateSize> analysis_state1{{0, 0, 0, 0, 0, 0}};
            std::array<int, StateSize> analysis_state2{{0, 0, 0, 0, 0, 0}};
            std::array<int, StateSize> synthesis_state1{{0, 0, 0, 0, 0, 0}};
            std::array<int, StateSize> synthesis_state2{{0, 0, 0, 0, 0, 0}};
        };
        BandsStates two_bands_state_;
    };

}

#endif //SMARTCORE_BAND_EXTRACTOR_HPP
