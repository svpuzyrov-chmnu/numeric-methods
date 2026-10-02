#include "qr-decomposition.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>

namespace linear_algebra::matrix::decomposition::qr
{
    MatrixDecompositionResult QRDecomposition::decompose(RectangleMatrix m) const
    {
        const auto rows = m.rows();
        const auto cols = m.cols();

        for (size_t row = 0; row < rows; ++row)
        {
            for (size_t col = 0; col < cols; ++col)
            {
                if (!std::isfinite(m(row, col)))
                {
                    throw std::invalid_argument("QR decomposition requires finite matrix entries.");
                }
            }
        }

        RectangleMatrix q(
            rows,
            rows,
            [](const size_t&, const size_t&) { return true; },
            [](const size_t& row, const size_t& col) { return row == col ? 1.0 : 0.0; }
        );

        for (size_t pivot = 0; pivot < std::min(rows, cols); ++pivot)
        {
            double norm = 0.0;
            for (size_t row = pivot; row < rows; ++row)
            {
                norm = std::hypot(norm, m(row, pivot));
            }

            if (!std::isfinite(norm))
            {
                throw std::runtime_error("QR decomposition encountered a non-finite norm.");
            }

            if (norm == 0.0)
            {
                continue;
            }

            const auto alpha = -std::copysign(norm, m(pivot, pivot));
            std::vector<double> reflector(rows - pivot);
            reflector[0] = (m(pivot, pivot) - alpha) / norm;
            for (size_t row = pivot + 1; row < rows; ++row)
            {
                reflector[row - pivot] = m(row, pivot) / norm;
            }

            double reflector_norm_squared = 0.0;
            for (const auto value : reflector)
            {
                reflector_norm_squared += value * value;
            }
            const auto beta = 2.0 / reflector_norm_squared;

            for (size_t col = pivot; col < cols; ++col)
            {
                double projection = 0.0;
                for (size_t row = pivot; row < rows; ++row)
                {
                    projection += reflector[row - pivot] * m(row, col);
                }

                projection *= beta;
                for (size_t row = pivot; row < rows; ++row)
                {
                    m(row, col) -= projection * reflector[row - pivot];
                    if (!std::isfinite(m(row, col)))
                    {
                        throw std::runtime_error("QR decomposition encountered a non-finite value.");
                    }
                }
            }

            for (size_t row = 0; row < rows; ++row)
            {
                double projection = 0.0;
                for (size_t col = pivot; col < rows; ++col)
                {
                    projection += q(row, col) * reflector[col - pivot];
                }

                projection *= beta;
                for (size_t col = pivot; col < rows; ++col)
                {
                    q(row, col) -= projection * reflector[col - pivot];
                    if (!std::isfinite(q(row, col)))
                    {
                        throw std::runtime_error("QR decomposition encountered a non-finite value.");
                    }
                }
            }
        }

        UpperTriangleMatrix r(rows, cols);
        for (size_t row = 0; row < rows; ++row)
        {
            for (size_t col = row; col < cols; ++col)
            {
                r(row, col) = m(row, col);
            }
        }

        return {std::move(q), std::move(r)};
    }
}
