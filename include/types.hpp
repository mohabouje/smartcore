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

    template <typename T>
    using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

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
