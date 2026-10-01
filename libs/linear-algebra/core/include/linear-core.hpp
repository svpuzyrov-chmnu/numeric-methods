#pragma once

#include <exception>
#include "rectangle-matrix.hpp"
#include "vector.hpp"

namespace linear_algebra::linear_core
{
    struct xInfinitySolution: std::exception
    {};

    struct xNoSolution: std::exception
    {};

    class ILinearSystemResolver
    {
    protected:
        static void validate_at(const matrix::RectangleMatrix&, const vector::Vector&, const size_t&);

        static void validate_dimensions(const matrix::RectangleMatrix&, const vector::Vector&);

        static void validate_matrix(const matrix::RectangleMatrix&);

        static void validate_vector(const matrix::RectangleMatrix&, const vector::Vector&);

        static void validate_finite_vector(const vector::Vector&);

        static void reorder_by_max(matrix::RectangleMatrix&, vector::Vector&);
    public:
        ILinearSystemResolver() = default;

        virtual ~ILinearSystemResolver() = default;

        [[nodiscard]] virtual vector::Vector resolve(const matrix::RectangleMatrix&, const vector::Vector&) const = 0;
        
        [[nodiscard]] virtual vector::Vector deviate(const matrix::RectangleMatrix&, const vector::Vector&, const vector::Vector&) const;
    };

    class IDeterminantResolver
    {
    public:
        IDeterminantResolver() = default;

        virtual ~IDeterminantResolver() = default;

        [[nodiscard]] virtual double determinant(const matrix::RectangleMatrix&) const = 0;
    };

    class IInverseMatrixResolver
    {
    public:
        IInverseMatrixResolver() = default;

        virtual ~IInverseMatrixResolver() = default;

        [[nodiscard]] virtual matrix::RectangleMatrix inverse(const matrix::RectangleMatrix&) const = 0;
    };

    double max_matrix_norm(const matrix::RectangleMatrix&);

    double max_vector_norm(const vector::Vector&);
}
