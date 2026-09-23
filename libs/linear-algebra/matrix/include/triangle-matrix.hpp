#pragma once
#include <vector>
#include "rectangle-matrix.hpp"
#include "exception.hpp"

namespace linear_algebra::matrix
{
    struct xTriangleMatrixUnchangeIndex : std::exception
    {
        const size_t source_;
        const size_t target_;
        explicit xTriangleMatrixUnchangeIndex(const size_t& source, const size_t& target) : source_(source), target_(target) {}
    };

    class TriangleMatrix : public RectangleMatrix
    {
    protected:
        [[nodiscard]] virtual bool is_over_defined(const size_t&, const size_t&) const = 0;

    public:
        TriangleMatrix(const size_t rows, const size_t cols,
                       const std::function<size_t(const size_t&, const size_t&)>& col_size_generator)
            : RectangleMatrix(rows, cols, col_size_generator)
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

        void change_rows(const size_t& i, const size_t& j) override
        {
            throw xTriangleMatrixUnchangeIndex{i, j};
        }

        void change_cols(const size_t& i, const size_t& j) override
        {
            throw xTriangleMatrixUnchangeIndex{i, j};
        }
    };
}
