#ifndef SMARTCORE_MIXED_LOW_PASS_HPP
#define SMARTCORE_MIXED_LOW_PASS_HPP

#include <memory>
#include <vector>

namespace score {

    class MixedLowPass {
    public:

        /**
         * @brief Creates a mixed low pass filter.
         * @param cut_off Cut off frequency in Hz.
         */
        explicit MixedLowPass(float cut_off);

        /**
         * @brief Default destructor
         */
        ~MixedLowPass();

        /**
         * @brief Returns the cut-off frequency of the low pass filter
         * @return Cut off frequency in Hz.
         */
        float cutOffFrequency() const;

        /**
         * @brief Sets the cut-off frequency of the low pass filter
         * @param cut_off Frequency in Hz.
         */
        void setCutOffFrequency(float cut_off);

        /**
         * @brief Down mix the input signals by performing an average low pass filtering
         * @param input  Input signals from different sources.
         * @param output Average low pass filtered signal.
         */
        void process(const std::vector<std::vector<float>>& input, std::vector<float>& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };


}

#endif //SMARTCORE_MIXED_LOW_PASS_HPP
