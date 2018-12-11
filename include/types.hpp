#ifndef SMARTCORE_TYPES_HPP
#define SMARTCORE_TYPES_HPP

#include <eigen3/Eigen/Core>
#include <vector>

namespace score {

    template <typename T>
    using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

    template <typename T>
    using Vector = std::vector<T>;

    template <typename T>
    using Tensor = std::vector<Matrix<T>>;
}

#endif //SMARTCORE_TYPES_HPP
