#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "vector.hpp"
#include "exception.hpp"

namespace linear_algebra::matrix
{
    class ViewMatrix
    {
    public:
        ViewMatrix() = default;

        virtual ~ViewMatrix() = default;

        [[nodiscard]] virtual double at(const size_t& i, const size_t& j) const = 0;

        virtual double operator()(const size_t& i, const size_t& j) const { return at(i, j); }

        virtual double& at(const size_t& i, const size_t& j) = 0;

        virtual double& operator()(const size_t& i, const size_t& j)
        {
            return at(i, j);
        }

        [[nodiscard]] virtual const size_t rows() const = 0;

        [[nodiscard]] virtual const size_t cols() const = 0;
    };

    class Matrix : public virtual ViewMatrix
    {
    protected:
        std::vector<std::shared_ptr<vector::Vector>> data_;

        void check_indices(const size_t&, const size_t&) const;

    public:
        Matrix(const size_t rows, const size_t cols,
               const std::function<size_t(const size_t&, const size_t&)>& col_size_generator)
            : data_(rows)
        {
            for (size_t i = 0; i < data_.size(); ++i)
            {
                auto col_size = col_size_generator(cols, i);
                data_.push_back(std::make_shared<vector::Vector>(col_size));
            }
        }

        Matrix(const size_t rows, const size_t cols)
            : Matrix(rows, cols, [](auto& c, auto& j) -> size_t { return c; })
        {
        }

        ~Matrix() override = default;

        [[nodiscard]] double at(const size_t& i, const size_t& j) const override
        {
            check_indices(i, j);
            return data_.at(i)->at(j);
        }

        double& operator()(const size_t& i, const size_t& j) override
        {
            check_indices(i, j);
            return data_.at(i)->at(j);
        }

        void change_rows(const size_t& source_row, const size_t& target_row);

        [[nodiscard]] const size_t rows() const final
        {
            return data_.size();
        }

        [[nodiscard]] const size_t cols() const final
        {
            return data_.at(0)->size();
        }
    };

    class SquareMatrix : public Matrix
    {
    public:
        ~SquareMatrix() override = default;

        SquareMatrix(const size_t size)
            : Matrix(size, size)
        {
        }
    };

    class TriangleMatrix : public Matrix
    {
    protected:
        [[nodiscard]] virtual bool is_over_defined(const size_t&, const size_t&) const = 0;
    public:
        TriangleMatrix(const size_t rows, const size_t cols,
                       const std::function<size_t(const size_t&, const size_t&)>& col_size_generator)
            : Matrix(rows, cols, col_size_generator)
        {
        }

        ~TriangleMatrix() override = default;

        [[nodiscard]] double at(const size_t& i, const size_t& j) const override
        {
            if (is_over_defined(i, j))
            {
                return 0;
            }

            return data_.at(i)->at(j);
        }

        double& operator()(const size_t& i, const size_t& j) override
        {
            if (is_over_defined(i, j))
            {
                throw exception::xOutOfRange{i, j};
            }

            return data_.at(i)->at(j);
        }
    };

    class UpperTriangleMatrix : public TriangleMatrix
    {
    protected:
        [[nodiscard]] bool is_over_defined(const size_t&, const size_t&) const override;
    public:
        UpperTriangleMatrix(const size_t rows, const size_t cols)
            : TriangleMatrix(rows, cols, [](const auto& c, const auto& j) -> size_t { return c - j; })
        {
        }

        ~UpperTriangleMatrix() override = default;
    };

    class LowerTriangleMatrix : public TriangleMatrix
    {
    protected:
        [[nodiscard]] bool is_over_defined(const size_t&, const size_t&) const override;
    public:
        LowerTriangleMatrix(const size_t rows, const size_t cols)
            : TriangleMatrix(rows, cols, [](const auto& c, const auto& j) -> size_t { return j + 1; })
        {
        }

        ~LowerTriangleMatrix() override = default;
    };
}
