#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "vector.hpp"
#include "matrix.hpp"

namespace linear_algebra::matrix
{
    class RectangleMatrix : public virtual Matrix
    {
        const size_t rows_;
        const size_t cols_;

    protected:
        std::vector<std::shared_ptr<vector::Vector>> data_;

        void check_indices(const size_t&, const size_t&) const;

        RectangleMatrix(const size_t rows, const size_t cols,
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

    public:
        RectangleMatrix(const size_t rows, const size_t cols)
            : RectangleMatrix(rows, cols, [](auto& c, auto& j) -> size_t { return c; })
        {
        }

        template<size_t rows, size_t cols>
        RectangleMatrix(const double (&data)[rows][cols])
            : rows_(rows)
              , cols_(cols)
              , data_(rows)
        {
            for (size_t i = 0; i < rows; ++i)
            {
                data_[i] = std::make_shared<vector::Vector>(cols);

                for (size_t j = 0; j < cols; ++j)
                {
                    data_[i]->at(j) = data[i][j];
                }
            }
        }

        RectangleMatrix(const size_t& rows, const size_t& cols,
                        const std::function<bool(const size_t&, const size_t&)>& index_predicate,
                        const std::function<double(const size_t&, const size_t&)>& value_generator,
                        const double& default_value = 0.0)
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

        ~RectangleMatrix() override = default;

        [[nodiscard]] double at(const size_t& i, const size_t& j) const override
        {
            check_indices(i, j);
            return data_.at(i)->at(j);
        }

        double& at(const size_t& i, const size_t& j) override
        {
            check_indices(i, j);
            return data_.at(i)->at(j);
        }

        void change_rows(const size_t&, const size_t&) override;

        void change_cols(const size_t&, const size_t&) override;

        [[nodiscard]] const size_t rows() const final
        {
            return rows_;
        }

        [[nodiscard]] const size_t cols() const final
        {
            return cols_;
        }
    };

    RectangleMatrix operator+(const RectangleMatrix& lhs, const RectangleMatrix& rhs);

    RectangleMatrix operator-(const RectangleMatrix& lhs, const RectangleMatrix& rhs);

    RectangleMatrix operator*(const RectangleMatrix& lhs, const RectangleMatrix& rhs);

    RectangleMatrix operator*(const RectangleMatrix& lhs, const double& rhs);
}
