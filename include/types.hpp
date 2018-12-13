#ifndef SMARTCORE_TYPES_HPP
#define SMARTCORE_TYPES_HPP

#include <eigen3/Eigen/Core>
#include <vector>
#include <array>

namespace score {

    /**
    * The Bands enum represent the different available bands in each channel.
    */
    enum Bands : unsigned int {
        Band0To8kHz = 0,
        Band8To16kHz,
        NumberBands
    };

    /**
     * The SampleRates enum represent the different supported sample rates.
     */
    enum SampleRates : unsigned int {
        SampleRate8kHz = 8000,
        SampleRate16kHz = 16000,
        SampleRate32kHz = 32000,
        SampleRate48kHz = 48000
    };

    constexpr auto MaxFloatS16 = std::numeric_limits<std::int16_t>::max();
    constexpr auto MinFloatS16 = std::numeric_limits<std::int16_t>::min();

    template <typename T>
    using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

    template <typename T>
    using Vector = std::vector<T>;

    template <typename T, std::size_t N>
    using Array = std::array<T, N>;

    template <typename T>
    using Tensor = std::vector<Matrix<T>>;

    template <typename T>
    using Point = std::array<double, 3>;
}

#endif //SMARTCORE_TYPES_HPP
