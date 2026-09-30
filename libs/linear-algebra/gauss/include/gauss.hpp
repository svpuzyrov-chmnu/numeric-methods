#pragma once

#include "rectangle-matrix.hpp"
#include "vector.hpp"

namespace linear_algebra::gauss
{
    class IGaussResolver
    {
    public:
        IGaussResolver() = default;

        virtual ~IGaussResolver() = default;

        [[nodiscard]] virtual vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const = 0;
        
        [[nodiscard]] virtual vector::Vector deviate(const matrix::RectangleMatrix&, const vector::Vector&, const vector::Vector&) const = 0;
        
        [[nodiscard]] virtual double determinant(const matrix::RectangleMatrix&) const = 0;

        [[nodiscard]] virtual matrix::RectangleMatrix inverse(const matrix::RectangleMatrix&) const = 0;

    };
}
