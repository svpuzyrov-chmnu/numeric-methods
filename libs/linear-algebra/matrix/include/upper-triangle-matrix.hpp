#pragma once
#include "triangle-matrix.hpp"

namespace linear_algebra::matrix
{
    class UpperTriangleMatrix : public TriangleMatrix
    {
    protected:
        [[nodiscard]] bool is_over_defined(const size_t&, const size_t&) const override;
        [[nodiscard]] size_t stored_col_index(const size_t& row, const size_t& col) const override
        {
            return col - row;
        }

    public:
        UpperTriangleMatrix(const size_t rows, const size_t cols)
            : TriangleMatrix(rows, cols, [&cols](const auto&, const auto& row) -> size_t
            {
                return row < cols ? cols - row : 0;
            })
        {
        }

        ~UpperTriangleMatrix() override = default;

        void init_row(const size_t&, const std::function<double(const size_t&, const size_t&)>&) override;
    };
}
