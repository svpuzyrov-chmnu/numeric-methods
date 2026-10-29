#pragma once

#include "interpolation-core.hpp"
#include "polynom.hpp"

namespace interpolation::list_squares
{

    class LeastSquares
    {
        core::Points points_;
    public:
        explicit LeastSquares(core::Points& points);

        [[nodiscard]] polynomial::Polynomial approx(const std::size_t&) const;

        [[nodiscard]] double squaredError(const polynomial::Polynomial& polynomial) const;
    };
}
