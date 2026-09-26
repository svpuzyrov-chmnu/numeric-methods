#pragma once

#include "gauss.hpp"

namespace linear_algebra::gauss
{
    class PartialGaussResolver : public GaussResolver
    {
        static void validate(const matrix::RectangleMatrix&, const vector::Vector&, const size_t&);
    public:
        PartialGaussResolver() = default;

        ~PartialGaussResolver() override = default;

        [[nodiscard]] vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const override;
    };
}
