#pragma once

#include "jacoby.hpp"

namespace linear_algebra::seidel
{
    class SeidelResolver : public jacoby::JacobyResolver
    {
    public:
        SeidelResolver() = default;

        ~SeidelResolver() override = default;

        [[nodiscard]] vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const override;
    };
}
