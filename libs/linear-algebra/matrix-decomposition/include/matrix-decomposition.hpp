#pragma once

#include "matrix-all.hpp"

namespace linear_algebra::matrix::decomposition
{
    template<typename T>
    class IMatrixDecomposition
    {
    public:
        IMatrixDecomposition() = default;

        virtual ~IMatrixDecomposition() = default;

        [[nodiscard]] virtual T decompose(RectangleMatrix) const = 0;
    };
}
