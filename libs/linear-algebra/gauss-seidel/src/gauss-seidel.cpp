#include "gauss-seidel.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace linear_algebra::gauss_seidel
{
    namespace
    {
        constexpr double tolerance = 1e-10;
        constexpr size_t max_iterations = 100000;
    }

    vector::Vector GaussSeidelResolver::resolve(const matrix::RectangleMatrix& m, const vector::Vector& v) const
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
            const vector::Vector previous_solution(solution);
            for (size_t row = 0; row < size; ++row)
            {
                auto value = scaled_rhs[row];

                for (size_t col = 0; col < row; ++col)
                {
                    value += iteration_matrix(row, col) * solution[col];
                }

                for (size_t col = row + 1; col < size; ++col)
                {
                    value += iteration_matrix(row, col) * previous_solution[col];
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
                throw std::runtime_error("Gauss-Seidel iteration diverged.");
            }

            const auto solution_norm = linear_core::max_vector_norm(solution);

            const auto rhs_norm = linear_core::max_vector_norm(rhs);

            const auto matrix_norm = linear_core::max_matrix_norm(work);

            const auto residual_tolerance = tolerance * (1.0 + rhs_norm + matrix_norm * solution_norm);

            if (!std::isfinite(residual_tolerance))
            {
                throw std::runtime_error("Gauss-Seidel iteration diverged.");
            }

            if (residual_norm <= residual_tolerance)
            {
                return solution;
            }
        }

        throw std::runtime_error("Gauss-Seidel iteration did not converge.");
    }
}
