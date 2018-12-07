#ifndef NOISE_SUPPRESSION_H
#define NOISE_SUPPRESSION_H

#include <vector>
#include <memory>

namespace score {

    class NoiseSuppression {
    public:

        ~NoiseSuppression();

        void process(const float* input, float* output, std::size_t numberSamples);
        void process(const std::vector<float> input, std::vector<float>& output);
    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };


}

#endif // NOISE_SUPPRESSION_H
