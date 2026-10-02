#pragma once

#include "matrix-all.hpp"
#include "matrix-decomposition.hpp"

#include <vector>

namespace linear_algebra::matrix::decomposition::lu
{
    struct MatrixDecompositionResult
    {
        LowerTriangleMatrix lower;
        UpperTriangleMatrix upper;
        // Row i of P*A comes from row pivot_indices[i] of A.
        std::vector<size_t> pivot_indices;
    };

    class LUDecomposition : public IMatrixDecomposition<MatrixDecompositionResult>
    {
    public:
        LUDecomposition() = default;

        ~LUDecomposition() override = default;

        [[nodiscard]] MatrixDecompositionResult decompose(RectangleMatrix) const override;
    };
}
