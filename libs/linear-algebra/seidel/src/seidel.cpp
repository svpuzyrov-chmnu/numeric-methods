#include "seidel.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace linear_algebra::seidel
{
    namespace
    {
        constexpr double tolerance = 1e-10;
        constexpr size_t max_iterations = 100000;
    }

    void SeidelResolver::validate_diagonal(const matrix::RectangleMatrix& m)
    {
        for (size_t row = 0; row < m.rows(); ++row)
        {
            if (!std::isfinite(m(row, row)) ||
                std::abs(m(row, row)) <= std::numeric_limits<double>::epsilon())
            {
                throw std::invalid_argument("Seidel iteration requires non-zero finite diagonal coefficients.");
            }
        }
    }

    matrix::RectangleMatrix SeidelResolver::transform_matrix(const matrix::RectangleMatrix& m)
    {
        validate_matrix(m);
        validate_diagonal(m);

        const auto size = m.rows();
        matrix::RectangleMatrix transformed(size, size);
        for (size_t row = 0; row < size; ++row)
        {
            for (size_t col = 0; col < size; ++col)
            {
                transformed(row, col) = row == col ? 0.0 : -m(row, col) / m(row, row);
            }
        }
        validate_matrix(transformed);

        return transformed;
    }

    vector::Vector SeidelResolver::transform_vector(const matrix::RectangleMatrix& m, const vector::Vector& v)
    {
        validate_matrix(m);
        validate_vector(m, v);
        validate_diagonal(m);

        vector::Vector transformed(v.size());
        for (size_t row = 0; row < v.size(); ++row)
        {
            transformed[row] = v[row] / m(row, row);
        }
        validate_finite_vector(transformed);
        return transformed;
    }

    double SeidelResolver::calculate_residual(const matrix::RectangleMatrix& work, const vector::Vector& rhs, const vector::Vector& solution)
    {
        const auto size = work.rows();

        double residual_norm = 0.0;

        for (size_t row = 0; row < size; ++row)
        {
            auto residual = rhs[row];

            for (size_t col = 0; col < size; ++col)
            {
                residual -= work(row, col) * solution[col];
            }

            residual_norm = std::max(residual_norm, std::abs(residual));
        }

        return residual_norm;
    }

    vector::Vector SeidelResolver::resolve(const matrix::RectangleMatrix& m, const vector::Vector& v) const
    {

        validate_dimensions(m, v);

        const auto size = m.rows();
        if (size == 0)
        {
            return vector::Vector(0);
        }

        matrix::RectangleMatrix work(m);

        vector::Vector rhs(v);

        validate_matrix(work);

        validate_vector(work, rhs);

        reorder_by_max(work, rhs);

        const auto iteration_matrix = transform_matrix(work);

        const auto scaled_rhs = transform_vector(work, rhs);

        vector::Vector solution(size);

        for (size_t iteration = 0; iteration < max_iterations; ++iteration)
        {
            for (size_t row = 0; row < size; ++row)
            {
                auto value = scaled_rhs[row];

                for (size_t col = 0; col < size; ++col)
                {
                    value += iteration_matrix(row, col) * solution[col];
                }
                if (!std::isfinite(value))
                {
                    throw std::runtime_error("Seidel iteration diverged.");
                }
                solution[row] = value;
            }

            const auto residual_norm =  calculate_residual(work, rhs, solution);

            if (!std::isfinite(residual_norm))
            {
                throw std::runtime_error("Seidel iteration diverged.");
            }

            const auto solution_norm = linear_core::max_vector_norm(solution);

            const auto rhs_norm = linear_core::max_vector_norm(rhs);

            const auto matrix_norm = linear_core::max_matrix_norm(work);

            const auto residual_tolerance = tolerance * (1.0 + rhs_norm + matrix_norm * solution_norm);

            if (!std::isfinite(residual_tolerance))
            {
                throw std::runtime_error("Seidel iteration diverged.");
            }

            if (residual_norm <= residual_tolerance)
            {
                return solution;
            }
        }

        throw std::runtime_error("Seidel iteration did not converge.");
    }
}
