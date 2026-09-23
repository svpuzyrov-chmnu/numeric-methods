#pragma once
#include <vector>
#include "diagonal-matrix.hpp"

namespace linear_algebra::matrix
{
    class IdentityMatrix : public DiagonalMatrix
    {
    public:
        IdentityMatrix(const size_t rows, const size_t cols)
            : DiagonalMatrix(rows, cols, [](auto&, auto&) { return 1.0; })
        {
        }

        ~IdentityMatrix() override = default;
    };
}
