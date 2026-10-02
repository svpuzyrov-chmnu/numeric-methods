#pragma once

#include "matrix-all.hpp"
#include "matrix-decomposition.hpp"

namespace linear_algebra::matrix::decomposition::qr
{
    struct MatrixDecompositionResult
    {
        RectangleMatrix q;
        UpperTriangleMatrix r;
    };

    class QRDecomposition : public IMatrixDecomposition<MatrixDecompositionResult>
    {
    public:
        QRDecomposition() = default;

        ~QRDecomposition() override = default;

        [[nodiscard]] MatrixDecompositionResult decompose(RectangleMatrix) const override;
    };
}
