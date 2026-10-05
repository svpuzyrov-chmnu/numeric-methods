#include "lu-decomposition.hpp"

#include <cmath>
#include <numeric>
#include <stdexcept>

namespace linear_algebra::matrix::decomposition::lu
{
    MatrixDecompositionResult LUDecomposition::decompose(RectangleMatrix m) const
    {
        if (m.rows() != m.cols())
        {
            throw std::invalid_argument("LU decomposition requires a square matrix.");
        }

        const auto size = m.rows();

        MatrixDecompositionResult result {
            LowerTriangleMatrix(size, size),
            UpperTriangleMatrix(size, size),
            std::vector<size_t>(size)
        };

        std::iota(result.pivot_indices.begin(), result.pivot_indices.end(), 0);

        for (size_t row = 0; row < size; ++row)
        {
            for (size_t col = 0; col < size; ++col)
            {
                if (!std::isfinite(m(row, col)))
                {
                    throw std::invalid_argument("LU decomposition requires finite matrix entries.");
                }
            }
            result.lower(row, row) = 1.0;
        }

        for (size_t pivot_col = 0; pivot_col < size; ++pivot_col)
        {
            const auto pivot_row = m.index_of_max_abs_in_col(pivot_col, pivot_col);

            if (m(pivot_row, pivot_col) == 0.0)
            {
                throw std::runtime_error("LU decomposition is undefined for a singular matrix.");
            }

            if (pivot_row != pivot_col)
            {
                m.change_rows(pivot_row, pivot_col);

                std::swap(result.pivot_indices[pivot_row], result.pivot_indices[pivot_col]);

                for (size_t col = 0; col < pivot_col; ++col)
                {
                    std::swap(result.lower(pivot_row, col), result.lower(pivot_col, col));
                }
            }

            for (size_t col = pivot_col; col < size; ++col)
            {
                result.upper(pivot_col, col) = m(pivot_col, col);
            }

            for (size_t row = pivot_col + 1; row < size; ++row)
            {
                const auto factor = -m(row, pivot_col) / result.upper(pivot_col, pivot_col);

                if (!std::isfinite(factor))
                {
                    throw std::runtime_error("LU decomposition encountered a non-finite factor.");
                }

                result.lower(row, pivot_col) = factor;

                for (size_t col = pivot_col + 1; col < size; ++col)
                {
                    m(row, col) += factor * result.upper(pivot_col, col);
                    if (!std::isfinite(m(row, col)))
                    {
                        throw std::runtime_error("LU decomposition encountered a non-finite value.");
                    }
                }
            }
        }

        return result;
    }
}
