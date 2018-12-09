#ifndef SMARTCORE_MIXED_LOW_PASS_HPP
#define SMARTCORE_MIXED_LOW_PASS_HPP

#include <memory>
#include <vector>

namespace score {

    class DownMix {
    public:

        /**
         * @brief Down mix the input signals
         * @param input  Input signals from different sources.
         * @param output Average low pass filtered signal.
         */
        void process(const std::vector<std::vector<std::int16_t>>& input, std::vector<std::int16_t>& output);

        /**
         * @brief Down mix the input signals
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
