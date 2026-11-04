#pragma once

#include "plane2d.hpp"
#include "polynom.hpp"

namespace interpolation::mean_square
{

	class MeanSquare
    {
        plane2d::Points points_;
    public:
        explicit MeanSquare(const plane2d::Points& points);

        [[nodiscard]] polynomial::Polynomial approx(const std::size_t&) const;

        [[nodiscard]] double squaredError(const polynomial::Polynomial& polynomial) const;
    };
}
