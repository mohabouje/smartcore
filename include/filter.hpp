#ifndef SMARTCORE_FILTER_HPP
#define SMARTCORE_FILTER_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Filter {
    private:

        /**
        * @brief The filter_type enum defines the different available filters.
        */
        enum class FilterType {
            LowPass,           /*!< Low pass filter */
            HighPass,          /*!< High pass filter */
            BandPass,          /*!< Band pass filter */
            BandStop,          /*!< Band stop filter */
            LowShelf,          /*!< Low shelf (shelving) filter */
            HighShelf,         /*!< High shelf (shelving) filter */
            BandShelf,         /*!< Band shelf (shelving) filter */
        };

        /**
         * @brief The DesignerType enum defines the different filter's implementation
         * from different authors.
         * @see filter_type
         */
        enum class DesignerType {
            Butterworth, /*!< Digital implementation of the classic Butterworth filter by using the bilinear transform */
            ChebyshevI, /*!< Digital implementation of the Chebyshev polynomials (ripple in the passband) filter by using the bilinear transform */
            ChebyshevII, /*!< Digital implementation of the "Inverse Chebyshev" filters (ripple in the stopband) by using the bilinear transform */
        };

        /**
         * @brief Creates a filter with the given configuration
         * @note The filter has to be configured after the cration
         * @param sample_rate Sampling frequency in Hz.
         * @param channels Number of channels.
         */
        Filter(std::int32_t sample_rate, std::int8_t channels);

        /**
         * @brief Default destructor
         */
        ~Filter();

        /**
         * @brief Set the filter type
         * @param type Type of the filter.
         */
        void setType(FilterType type);

        /**
         * @brief Returns the current filter type
         * @return Type of the internal filter.
         */
        FilterType type() const;

        /**
         * @brief Sets the filter designer
         * @param designer Designer type
         */
        void setDesigner(DesignerType designer);

        /**
         * @brief Returns the filter designer type.
         * @return Designer type
         */
        DesignerType designer() const;

        /**
         * @brief Returns the order of the filter
         */
        std::size_t order() const;

        /**
         * @brief Sets the order of the filter
         * @note Maximum allowed order: 100
         * @param order Order of the filter.
         */
        void setOrder(std::size_t order);

        /**
         * @brief Returns the cut off frequency
         * @return Frequency in Hz.
         */
        float cutOff() const;

        /**
         * @brief Sets the cut off frequency of the low/high pass filter
         * @param frequency Frequency in Hz.
         */
        void setCutOff(float frequency);

        /**
         * @brief Returns the band's ripple
         * @return Band ripple in dBs.
         */
        float ripple() const;

        /**
         * @brief Sets the ripple of the band in dBs,
         * @param ripple_db Ripple in dBs.
         */
        void setRipple(float ripple_db);

        /**
         * @brief Returns the gain of the shelving filter.
         * @return Gain of the shelving filter.
         */
        float gain() const;

        /**
         * @brief Sets the gain of the shelving filter.
         * @param gain Gain of the shelving filter.
         */
        void setGain(float gain);

        /**
         * @brief Returns the center frequency of the band pass/stop filter
         * @return Center frequency in Hz.
         */
        float centerFrequency() const;

        /**
         * @brief Sets the center frequency of the band pass/stop filter
         * @param center_frequency Center frequency in Hz.
         */
        void setCenterFrequency(float center_frequency);

        /**
         * @brief Returns the bandwidth of the band pass/stop filter.
         * @return Bandwidth in Hz.
         */
        float bandwidth() const;

        /**
         * @brief Sets the bandwidth of the band pass/stop filter
         * @param band Bandwidth in Hz.
         */
        void setBandwidth(float band);

        /**
         * @brief Resets the internal states
         */
        void reset();

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

#endif //SMARTCORE_FILTER_HPP
