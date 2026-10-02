#pragma once
#include <vector>
#include "triangle-matrix.hpp"

namespace linear_algebra::matrix
{
    class LowerTriangleMatrix : public TriangleMatrix
    {
    protected:
        [[nodiscard]] bool is_over_defined(const size_t&, const size_t&) const override;
        [[nodiscard]] size_t stored_col_index(const size_t&, const size_t& col) const override
        {
            return col;
        }

    public:
        LowerTriangleMatrix(const LowerTriangleMatrix&) = default;

        LowerTriangleMatrix(LowerTriangleMatrix&&) = default;

        LowerTriangleMatrix(const size_t rows, const size_t cols)
            : TriangleMatrix(rows, cols, [&cols](const auto&, const auto& row) -> size_t
            {
                return row < cols ? row + 1 : cols;
            })
        {
        }

        ~LowerTriangleMatrix() override = default;

        void init_row(const size_t&, const std::function<double(const size_t&, const size_t&)>&) override;
    };
}
