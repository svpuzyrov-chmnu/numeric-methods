#pragma once

#include "seidel.hpp"

namespace linear_algebra::gauss_seidel
{
    class GaussSeidelResolver : public seidel::SeidelResolver
    {
    public:
        GaussSeidelResolver() = default;

        ~GaussSeidelResolver() override = default;

        [[nodiscard]] vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const override;
    };
}
