#include <cmath>
#include "rectangle-matrix.hpp"
#include "exception.hpp"

namespace linear_algebra::matrix
{
    void RectangleMatrix::check_indices(const size_t& i, const size_t& j) const
    {
        if (i < 0 || i >= data_.size())
        {
            throw exception::xInvalidIndex(i);
        }

        if (j < 0 || j >= data_.at(0)->size())
        {
            throw exception::xInvalidIndex(j);
        }
    }

    void RectangleMatrix::change_rows(const size_t& i, const size_t& j)
    {
        check_indices(i, 0);
        check_indices(j, 0);

        if (i != j)
        {
            const auto source_row_ref = data_.at(i);
            const auto target_row_ref = data_.at(j);

            data_.at(i) = target_row_ref;

            data_.at(j) = source_row_ref;
        }
    }

    void RectangleMatrix::change_cols(const size_t& i, const size_t& j)
    {
        check_indices(0, i);
        check_indices(0, j);

        if (i != j)
        {
            for (const auto& row_vector : data_)
            {
                std::swap(row_vector->at(i), row_vector->at(j));
            }
        }
    }

    static auto true_index_predicate = [](const size_t& i, const size_t& j) { return true; };

    RectangleMatrix operator+(const RectangleMatrix& lhs, const RectangleMatrix& rhs)
    {
        const auto rows = std::min(lhs.rows(), rhs.rows());
        const auto cols = std::min(lhs.cols(), rhs.cols());

        RectangleMatrix result(rows, cols,
                               true_index_predicate,
                               [&lhs, &rhs](const size_t& i, const size_t& j) { return lhs(i, j) + rhs(i, j); }
        );

        return result;
    }

    RectangleMatrix operator-(const RectangleMatrix& lhs, const RectangleMatrix& rhs)
    {
        const auto rows = std::min(lhs.rows(), rhs.rows());
        const auto cols = std::min(lhs.cols(), rhs.cols());

        RectangleMatrix result(rows, cols,
                               true_index_predicate,
                               [&lhs, &rhs](auto& i, auto& j) { return lhs(i, j) - rhs(i, j); }
        );

        return result;
    }

    RectangleMatrix operator*(const RectangleMatrix& lhs, const RectangleMatrix& rhs)
    {
        if (lhs.cols() != rhs.rows())
        {
            throw exception::xNonEqualMatrixDimensions(lhs.cols(), rhs.rows());
        }

        const auto rows = lhs.rows();
        const auto cols = rhs.cols();

        auto value_generator = [&lhs, &rhs](auto& i, auto& j)
        {
            double sum = 0.0;
            for (size_t k = 0; k < lhs.cols(); ++k)
            {
                sum += lhs(i, k) * rhs(k, j);
            }
            return sum;
        };

        RectangleMatrix result(rows, cols, true_index_predicate, value_generator);

        return result;
    }

    RectangleMatrix operator*(const RectangleMatrix& lhs, const double& rhs)
    {
        RectangleMatrix result(lhs.rows(), lhs.cols(), true_index_predicate,
                               [&lhs, &rhs](const size_t& i, const size_t& j) { return lhs(i, j) * rhs; }
        );

        return result;
    }
}
