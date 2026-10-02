#include "band-resolver.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace linear_algebra::band
{
    vector::Vector BandResolver::resolve(const matrix::band::BandMatrix<double>& matrix, const vector::Vector& rhs)
    {
        const auto size = matrix.size();

        if (rhs.size() != size)
        {
            throw std::invalid_argument("Right-hand side size must match matrix size.");
        }

        for (size_t row = 0; row < size; ++row)
        {
            if (!std::isfinite(rhs[row]))
            {
                throw std::invalid_argument("Band system requires finite right-hand-side values.");
            }
        }

        std::vector<double> coefficients(size * size);

        for (size_t row = 0; row < size; ++row)
        {
            for (size_t col = 0; col < size; ++col)
            {
                const auto value = matrix(row, col);

                if (!std::isfinite(value))
                {
                    throw std::invalid_argument("Band system requires finite matrix entries.");
                }

                coefficients[row * size + col] = value;
            }
        }

        vector::Vector transformed_rhs(rhs);

        for (size_t pivot_col = 0; pivot_col < size; ++pivot_col)
        {
            auto pivot_row = pivot_col;

            for (size_t row = pivot_col + 1; row < size; ++row)
            {
                if (std::abs(coefficients[row * size + pivot_col]) >
                    std::abs(coefficients[pivot_row * size + pivot_col]))
                {
                    pivot_row = row;
                }
            }

            if (const auto pivot = coefficients[pivot_row * size + pivot_col]; pivot == 0.0)
            {
                throw std::runtime_error("Band system is singular.");
            }

            if (pivot_row != pivot_col)
            {
                for (size_t col = pivot_col; col < size; ++col)
                {
                    std::swap(coefficients[pivot_row * size + col], coefficients[pivot_col * size + col]
                    );
                }

                std::swap(transformed_rhs[pivot_row], transformed_rhs[pivot_col]);
            }

            for (size_t row = pivot_col + 1; row < size; ++row)
            {
                const auto factor = coefficients[row * size + pivot_col] / coefficients[pivot_col * size + pivot_col];

                if (!std::isfinite(factor))
                {
                    throw std::runtime_error("Band elimination produced a non-finite factor.");
                }

                coefficients[row * size + pivot_col] = 0.0;

                for (size_t col = pivot_col + 1; col < size; ++col)
                {
                    coefficients[row * size + col] -= factor * coefficients[pivot_col * size + col];

                    if (!std::isfinite(coefficients[row * size + col]))
                    {
                        throw std::runtime_error("Band elimination produced a non-finite value.");
                    }
                }

                transformed_rhs[row] -= factor * transformed_rhs[pivot_col];

                if (!std::isfinite(transformed_rhs[row]))
                {
                    throw std::runtime_error("Band elimination produced a non-finite right-hand side.");
                }
            }
        }

        vector::Vector solution(size);

        for (size_t row = size; row > 0; --row)
        {
            const auto current_row = row - 1;

            auto value = transformed_rhs[current_row];

            for (size_t col = current_row + 1; col < size; ++col)
            {
                value -= coefficients[current_row * size + col] * solution[col];
            }

            const auto pivot = coefficients[current_row * size + current_row];

            if (pivot == 0.0)
            {
                throw std::runtime_error("Band system is singular.");
            }

            solution[current_row] = value / pivot;

            if (!std::isfinite(solution[current_row]))
            {
                throw std::runtime_error("Band solve produced a non-finite solution.");
            }
        }

        return solution;
    }
}
