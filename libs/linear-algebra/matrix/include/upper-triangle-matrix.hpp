#pragma once
#include "triangle-matrix.hpp"

namespace linear_algebra::matrix
{
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
}
