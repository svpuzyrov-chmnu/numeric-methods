#pragma once

#include "interpolation-core.hpp"
#include "polynom.hpp"

namespace interpolation::mean_square
{

	class MeanSquare
    {
        core::Points points_;
    public:
        explicit MeanSquare(core::Points& points);

        [[nodiscard]] polynomial::Polynomial approx(const std::size_t&) const;

        [[nodiscard]] double squaredError(const polynomial::Polynomial& polynomial) const;
    };
}
