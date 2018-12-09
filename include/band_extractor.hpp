#ifndef SMARTCORE_BAND_EXTRACTOR_HPP
#define SMARTCORE_BAND_EXTRACTOR_HPP

#include <memory>
#include <vector>
#include <cstring>

namespace score {

    class BandExtractor {
    public:
        static constexpr std::size_t NumBands = 2ul;

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
        void process(const std::vector<std::int16_t>& input, std::vector<std::vector<std::int16_t>>& output);

        /**
         * @brief Merges the low/high band in to a single audio frame.
         * @param bands Input buffer storing the different bands.
         * @param output Generated audio frame.
         */
        void synthesis(const std::vector<std::vector<std::int16_t>> &bands, std::vector<std::int16_t>& output);

    private:
        struct BandsStates {
            BandsStates() {
                reset();
            }

            void reset() {
                memset(analysis_state1, 0, sizeof(analysis_state1));
                memset(analysis_state2, 0, sizeof(analysis_state2));
                memset(synthesis_state1, 0, sizeof(synthesis_state1));
                memset(synthesis_state2, 0, sizeof(synthesis_state2));
            }

            static constexpr auto StateSize = 6;
            int analysis_state1[StateSize];
            int analysis_state2[StateSize];
            int synthesis_state1[StateSize];
            int synthesis_state2[StateSize];
        };
        BandsStates two_bands_state_;
    };

}

#endif //SMARTCORE_BAND_EXTRACTOR_HPP
