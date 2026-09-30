#include "partial-gauss.hpp"
#include "gauss-exception.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "identity-matrix.hpp"

namespace linear_algebra::gauss
{
    void PartialGaussResolver::validate(const matrix::RectangleMatrix& m, const vector::Vector& v, const size_t& row)
    {
        if (!std::isfinite(v[row]) || !std::isfinite(m(row, row)))
        {
            throw std::invalid_argument("Gauss solver requires all finite items.");
        }

        if (std::abs(m(row, row)) <= std::numeric_limits<double>::epsilon())
        {
            if (std::abs(v[row]) > std::numeric_limits<double>::epsilon())
            {
                throw exception::xNoSolution();
            }
            throw exception::xInfinitySolution();
        }
    }

    vector::Vector PartialGaussResolver::resolve(const matrix::RectangleMatrix& m, const vector::Vector& v) const
    {
        if (m.rows() != m.cols() || v.size() != m.rows())
        {
            throw std::invalid_argument("Gauss solver requires a square matrix and a matching vector.");
        }

        const auto size = m.rows();

        if (size == 0)
        {
            return vector::Vector(0);
        }

        matrix::RectangleMatrix work(m);

        vector::Vector rhs(v);

        for (size_t row = 0; row < size; ++row)
        {
            if (auto pivot_row = work.index_of_max_abs_in_col(row, row); pivot_row > row)
            {
                work.change_rows(pivot_row, row);
                std::swap(rhs[pivot_row], rhs[row]);
            }

            validate(work, rhs, row);

            for (size_t next_row = row + 1; next_row < size; ++next_row)
            {
                const auto factor = work(next_row, row) / work(row, row);

                work(next_row, row) = 0.0;

                for (size_t col = row + 1; col < size; ++col)
                {
                    work(next_row, col) -= factor * work(row, col);
                }
                rhs[next_row] -= factor * rhs[row];
            }
        }

        validate(work, rhs, size - 1);

        vector::Vector solution(size);

        for (size_t row = size; row > 0; --row)
        {
            auto value = rhs[row - 1];

            for (size_t col = row; col < size; ++col)
            {
                value -= work(row - 1, col) * solution[col];
            }

            solution[row - 1] = value / work(row - 1, row - 1);
        }

        return solution;
    }

    vector::Vector PartialGaussResolver::deviate(const matrix::RectangleMatrix& m, const vector::Vector& x, const vector::Vector& b) const
    {
        auto b1 = m * x;

        return b - b1;
    }

    double PartialGaussResolver::determinant(const matrix::RectangleMatrix& m) const
    {
        if (m.rows() != m.cols())
        {
            throw std::invalid_argument("Determinant is defined only for a square matrix.");
        }

        const auto size = m.rows();

        if (size == 0)
        {
            return 0.0;
        }

        matrix::RectangleMatrix work(m);

        int sign = 1;

        for (size_t row = 0; row < size; ++row)
        {
            if (auto pivot_row = work.index_of_max_abs_in_col(row, row); pivot_row > row)
            {
                work.change_rows(pivot_row, row);
                sign = -sign;
            }

            if (std::abs(m(row, row)) <= std::numeric_limits<double>::epsilon())
            {
                return 0.0;
            }

            for (size_t next_row = row + 1; next_row < size; ++next_row)
            {
                const auto factor = work(next_row, row) / work(row, row);

                work(next_row, row) = 0.0;

                for (size_t col = row + 1; col < size; ++col)
                {
                    work(next_row, col) -= factor * work(row, col);
                }
            }
        }

        double result = 1.0;

        for (size_t row = 0; row < size; ++row)
        {
            result *= work(row, row);
        }

        return sign * result;
    }

    matrix::RectangleMatrix PartialGaussResolver::inverse(const matrix::RectangleMatrix& m) const
    {
        if (m.rows() != m.cols())
        {
            throw std::invalid_argument("Inverse matrix is defined only for a square matrix.");
        }

        const auto size = m.rows();

        matrix::RectangleMatrix work(m);
        matrix::IdentityMatrix identity(size, size);

        for (size_t row = 0; row < size; ++row)
        {
            if (auto pivot_row = work.index_of_max_abs_in_col(row, row); pivot_row > row)
            {
                work.change_rows(pivot_row, row);
            }

            if (std::abs(m(row, row)) <= std::numeric_limits<double>::epsilon())
            {
                throw std::runtime_error("Inverse matrix is not defined.");
            }

            for (size_t next_row = row + 1; next_row < size; ++next_row)
            {
                const auto factor = work(next_row, row) / work(row, row);

                work(next_row, row) = 0.0;

                for (size_t col = row + 1; col < size; ++col)
                {
                    work(next_row, col) -= factor * work(row, col);
                }

                for (size_t col = 0; col < size; ++col)
                {
                    identity(next_row, col) -= factor * identity(row, col);
                }
            }
        }

        for (size_t row = size; row > 0; --row)
        {
            for (size_t next_row = row - 1; next_row > 0; --next_row)
            {
                const auto factor = work(next_row, row - 1) / work(row - 1, row - 1);

                work(next_row, row - 1) = 0.0;

                for (size_t col = row; col > 0; --col)
                {
                    work(next_row - 1, col - 1) -= factor * work(row - 1, col - 1);
                }

                for (size_t col = 0; col < size; ++col)
                {
                    identity(next_row, col) -= factor * identity(row, col);
                }
            }
        }

        for (size_t row = 0; row < size; ++row)
        {
            for (size_t col = 0; col < size; ++col)
            {
                identity(row, col) /= work(row, col);
            }
        }

        return matrix::RectangleMatrix{identity};
    }
}
