#ifndef SMARTCORE_FIR_HPP
#define SMARTCORE_FIR_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {
    
    class FIR {

        /**
         * @brief Creates a FIR filter with the given coefficients
         * @param channels Number of channels
         * @param coefficients Coefficients of the FIR filter
         * @param size Number of coefficients
         */
        FIR(std::int8_t channels, const float* coefficients, std::size_t size);
        
        /**
         * @brief Default destructor
         */
        ~FIR();
        
        /**
         * @brief Returns the coefficients of the FIR filter
         * @return Array of length Order storing the coefficients.
         */
        const float* coefficients() const;
        
        /**
         * @brief Returns the order of the FIR filter
         * @return Order of the FIR filter
         */
        std::size_t order() const;

        /**
         * @brief Filter the audio frame.
         *
         * @param input Buffer storing the input audio samples.
         * @param output Buffer storing the output audio samples.
         */
        void process(const AudioBuffer& input, AudioBuffer& output);
        
    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };
    
    
    
}


#endif //SMARTCORE_FIR_HPP
