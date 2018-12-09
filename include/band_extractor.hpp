#ifndef SMARTCORE_BAND_EXTRACTOR_HPP
#define SMARTCORE_BAND_EXTRACTOR_HPP

#include <memory>
#include <vector>

namespace score {

    class BandExtractor {
    public:

        /**
         * @brief Default constructor
         */
        BandExtractor();

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


        /**
         * @brief Splits the input buffer in two different bands (low/high).
         * @param input Input buffer.
         * @param output Output buffer storing the different bands.
         */
        void process(const std::vector<float>& input, std::vector<std::vector<float>>& output);

        /**
         * @brief Merges the low/high band in to a single audio frame.
         * @param bands Input buffer storing the different bands.
         * @param output Generated audio frame.
         */
        void synthesis(const std::vector<std::vector<float>> &bands, std::vector<float>& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };

}

#endif //SMARTCORE_BAND_EXTRACTOR_HPP
