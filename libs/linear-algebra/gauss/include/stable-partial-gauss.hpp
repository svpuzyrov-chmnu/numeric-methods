#pragma once

#include "linear-core.hpp"

namespace linear_algebra::gauss
{
    class StablePartialGaussResolver : public linear_core::ILinearSystemResolver
    {
    public:
        StablePartialGaussResolver() = default;

        ~StablePartialGaussResolver() override = default;

        [[nodiscard]] vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const override;

    };

    class StablePartialGaussDeterminantResolver : public linear_core::IDeterminantResolver
    {
    public:
        StablePartialGaussDeterminantResolver() = default;

        ~StablePartialGaussDeterminantResolver() override = default;

        [[nodiscard]] double determinant(const matrix::RectangleMatrix&) const override;
    };

    class StablePartialGaussInverseMatrixResolver : public linear_core::IInverseMatrixResolver
    {
    public:
        StablePartialGaussInverseMatrixResolver() = default;

        ~StablePartialGaussInverseMatrixResolver() override = default;

        [[nodiscard]] matrix::RectangleMatrix inverse(const matrix::RectangleMatrix&) const override;
    };
}
