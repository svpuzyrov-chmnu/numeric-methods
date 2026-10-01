#include "linear-core.hpp"
#include <cmath>
namespace linear_algebra::linear_core
{
    vector::Vector ILinearSystemResolver::deviate(const matrix::RectangleMatrix& m, const vector::Vector& x, const vector::Vector& b) const
    {
        const auto b1 = m * x;

        return b - b1;
    }

    void ILinearSystemResolver::validate_at(const matrix::RectangleMatrix& m, const vector::Vector& v, const size_t& row)
    {
        if (!std::isfinite(v[row]) || !std::isfinite(m(row, row)))
        {
            throw std::invalid_argument("Gauss solver requires all finite items.");
        }

        if (std::abs(m(row, row)) <= std::numeric_limits<double>::epsilon())
        {
            if (std::abs(v[row]) > std::numeric_limits<double>::epsilon())
            {
                throw xNoSolution();
            }
            throw xInfinitySolution();
        }
    }

    void ILinearSystemResolver::validate_dimensions(const matrix::RectangleMatrix& m, const vector::Vector& v)
    {
        if (m.rows() != m.cols() || v.size() != m.rows())
        {
            throw std::invalid_argument("Seidel solver requires a square matrix and a matching vector.");
        }
    }

    void ILinearSystemResolver::validate_matrix(const matrix::RectangleMatrix& m)
    {
        if (m.rows() != m.cols())
        {
            throw std::invalid_argument("Seidel solver requires a square matrix.");
        }

        for (size_t row = 0; row < m.rows(); ++row)
        {
            for (size_t col = 0; col < m.cols(); ++col)
            {
                if (!std::isfinite(m(row, col)))
                {
                    throw std::invalid_argument("Seidel solver requires finite matrix coefficients.");
                }
            }
        }
    }

    void ILinearSystemResolver::validate_vector(const matrix::RectangleMatrix& m, const vector::Vector& v)
    {
        if (v.size() != m.rows())
        {
            throw std::invalid_argument("Seidel solver requires a matching right-hand-side vector.");
        }

        validate_finite_vector(v);
    }

    void ILinearSystemResolver::validate_finite_vector(const vector::Vector& v)
    {
        for (size_t row = 0; row < v.size(); ++row)
        {
            if (!std::isfinite(v[row]))
            {
                throw std::invalid_argument("Seidel solver requires finite right-hand-side values.");
            }
        }
    }

    void ILinearSystemResolver::reorder_by_max(matrix::RectangleMatrix& work, vector::Vector& rhs)
    {
        const auto size = work.rows();

        for (size_t row = 0; row < size; ++row)
        {
            if (std::abs(work(row, row)) <= std::numeric_limits<double>::epsilon())
            {
                if (const auto pivot_row = work.index_of_max_abs_in_col(row, row); std::abs(work(pivot_row, row)) > std::numeric_limits<double>::epsilon())
                {
                    work.change_rows(pivot_row, row);
                    std::swap(rhs[pivot_row], rhs[row]);
                }
            }
        }
    }

    double max_matrix_norm(const matrix::RectangleMatrix& m)
    {
        double norm = 0.0;
        for (size_t row = 0; row < m.rows(); ++row)
        {
            double row_norm = 0.0;
            for (size_t col = 0; col < m.cols(); ++col)
            {
                row_norm += std::abs(m(row, col));
            }
            norm = std::max(norm, row_norm);
        }
        return norm;
    }

    double max_vector_norm(const vector::Vector& v)
    {
        double norm = 0.0;

        for (size_t i = 0; i < v.size(); ++i)
        {
            norm = std::max(norm, std::abs(v[i]));
        }
        return norm;
    }
}
