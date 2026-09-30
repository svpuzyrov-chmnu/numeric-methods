#pragma once

#include "gauss.hpp"

namespace linear_algebra::gauss
{
    class PartialGaussResolver : public IGaussResolver
    {
        static void validate(const matrix::RectangleMatrix&, const vector::Vector&, const size_t&);
    public:
        PartialGaussResolver() = default;

        ~PartialGaussResolver() override = default;

        [[nodiscard]] vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const override;
        
        [[nodiscard]] vector::Vector deviate(const matrix::RectangleMatrix&, const vector::Vector&, const vector::Vector&) const override;

        [[nodiscard]] double determinant(const matrix::RectangleMatrix&) const override;

        [[nodiscard]] matrix::RectangleMatrix inverse(const matrix::RectangleMatrix&) const override;
    };
}
