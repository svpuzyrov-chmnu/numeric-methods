#pragma once
#include <vector>
#include <functional>
#include "rectangle-matrix.hpp"

namespace linear_algebra::matrix
{
    class DiagonalMatrix : public RectangleMatrix
    {
    public:
        DiagonalMatrix(const size_t rows, const size_t cols,
                       const std::function<double(const size_t&, const size_t&)>& value_generator)
            : RectangleMatrix(rows, cols, [](auto& i, auto& j) { return i == j; }, value_generator)
        {
        }

        DiagonalMatrix(const size_t rows, const size_t cols, const double& value)
            : DiagonalMatrix(rows, cols, [&value](auto&, auto&) { return value; })
        {
        }

        ~DiagonalMatrix() override = default;
    };
}
