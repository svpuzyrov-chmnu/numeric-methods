#pragma once

#include "linear-core.hpp"

namespace linear_algebra::jacoby
{
    class JacobyResolver : public linear_core::ILinearSystemResolver
    {
    protected:
        static void validate_diagonal(const matrix::RectangleMatrix&);

        [[nodiscard]] static matrix::RectangleMatrix transform_matrix(const matrix::RectangleMatrix&);

        [[nodiscard]] static vector::Vector transform_vector(const matrix::RectangleMatrix&, const vector::Vector&);

        [[nodiscard]] static double calculate_residual(const matrix::RectangleMatrix&, const vector::Vector&, const vector::Vector&);
    public:
        JacobyResolver() = default;

        ~JacobyResolver() override = default;

        [[nodiscard]] vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const override;
    };
}
