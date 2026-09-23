#pragma once
#include "rectangle-matrix.hpp"

namespace linear_algebra::matrix
{
    class SquareMatrix : public RectangleMatrix
    {
    public:
        ~SquareMatrix() override = default;

        SquareMatrix(const size_t size)
            : RectangleMatrix(size, size)
        {
        }
    };
}
