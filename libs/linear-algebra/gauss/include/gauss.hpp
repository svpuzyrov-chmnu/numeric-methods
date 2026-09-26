#pragma once

#include "rectangle-matrix.hpp"
#include "vector.hpp"

namespace linear_algebra::gauss
{
    class GaussResolver
    {
    public:
        GaussResolver() = default;

        virtual ~GaussResolver() = default;

        [[nodiscard]] virtual vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const = 0;
    };
}
