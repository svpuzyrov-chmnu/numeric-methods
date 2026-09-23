#pragma once
#include <vector>
#include "triangle-matrix.hpp"

namespace linear_algebra::matrix
{
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
