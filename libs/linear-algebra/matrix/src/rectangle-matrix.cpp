#include <cmath>
#include "rectangle-matrix.hpp"
#include "matrix-exception.hpp"

namespace linear_algebra::matrix
{
    RectangleMatrix::RectangleMatrix(const size_t& rows, const size_t& cols,
                                     const std::function<size_t(const size_t&, const size_t&)>& col_size_generator)
        : rows_(rows)
          , cols_(cols)
          , data_(rows)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            const auto col_size = col_size_generator(cols, i);
            data_[i] = std::make_shared<vector::Vector>(col_size);
        }
    }

    RectangleMatrix::RectangleMatrix(const RectangleMatrix& rhs)
        : rows_(rhs.rows_)
          , cols_(rhs.cols_)
          , data_(rhs.data_)
    {
        for (size_t i = 0; i < rows_; ++i)
        {
            data_[i] = std::make_shared<vector::Vector>(cols_);
            for (size_t j = 0; j < cols_; ++j)
            {
                data_.at(i)->at(j) = rhs.data_.at(i)->at(j);
            }
        }
    }

    RectangleMatrix::RectangleMatrix(const size_t& rows, const size_t& cols,
                                     const std::function<bool(const size_t&, const size_t&)>& index_predicate,
                                     const std::function<double(const size_t&, const size_t&)>& value_generator,
                                     const double& default_value)
        : rows_(rows)
          , cols_(cols)
          , data_(rows)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            data_[i] = std::make_shared<vector::Vector>(cols);

            for (size_t j = 0; j < cols; ++j)
            {
                if (index_predicate(i, j))
                {
                    data_[i]->at(j) = value_generator(i, j);
                }
                else
                {
                    data_[i]->at(j) = default_value;
                }
            }
        }
    }

    void RectangleMatrix::check_indices(const size_t& i, const size_t& j) const
    {
        if (i >= rows_)
        {
            throw exception::xInvalidIndex(i);
        }

        if (j >= cols_)
        {
            throw exception::xInvalidIndex(j);
        }
    }

    size_t RectangleMatrix::index_of_max_abs_in_col(const size_t& col, const size_t& start_row) const
    {
        check_indices(0, col);
        check_indices(start_row, 0);

        size_t max_index = start_row;
        double max_value = std::abs(data_.at(start_row)->at(col));

        for (size_t i = start_row + 1; i < rows_; ++i)
        {
            if (const double current_value = std::abs(data_.at(i)->at(col)); current_value > max_value)
            {
                max_value = current_value;
                max_index = i;
            }
        }

        return max_index;
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

    vector::Vector operator*(const RectangleMatrix& lhs, const vector::Vector& rhs)
    {
        if (lhs.cols() != rhs.size())
        {
            throw std::invalid_argument("matrix column and vector size mismatch");
        }

        vector::Vector result(rhs.size());

        for (size_t row = 0; row < lhs.rows(); ++row)
        {
            result[row] = 0.0;

            for (size_t col = 0; col < lhs.cols(); ++col)
            {
                result[row] += lhs(row, col) * rhs[col];
            }
        }

        return result;
    }
}
