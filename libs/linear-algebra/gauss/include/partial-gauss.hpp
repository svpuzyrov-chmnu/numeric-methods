#pragma once

#include "linear-core.hpp"

namespace linear_algebra::gauss
{
    class PartialGaussResolver : public linear_core::ILinearSystemResolver
    {
    public:
        PartialGaussResolver() = default;

        ~PartialGaussResolver() override = default;

        [[nodiscard]] vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const override;
        
        [[nodiscard]] vector::Vector deviate(const matrix::RectangleMatrix&, const vector::Vector&, const vector::Vector&) const override;

    };

    class PartialGaussDeterminantResolver : public linear_core::IDeterminantResolver
    {
    public:
        PartialGaussDeterminantResolver() = default;

        ~PartialGaussDeterminantResolver() override = default;

        [[nodiscard]] double determinant(const matrix::RectangleMatrix&) const override;
    };

    class PartialGaussInverseMatrixResolver : public linear_core::IInverseMatrixResolver
    {
    public:
        PartialGaussInverseMatrixResolver() = default;

        ~PartialGaussInverseMatrixResolver() override = default;

        [[nodiscard]] matrix::RectangleMatrix inverse(const matrix::RectangleMatrix&) const override;
    };
}
