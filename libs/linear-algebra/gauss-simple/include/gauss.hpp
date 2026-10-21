#pragma once

#include <exception>
#include "matrix.hpp"

namespace linear_algebra::gauss::simple
{
    using matrix::simple::Matrix;

    using matrix::simple::Vector;

    struct xNonSquareMatrix: std::exception {};

    struct xMismatchRightSideSize: std::exception {};

    struct xNoSolution: std::exception {};

    struct xInfiniteSolution: std::exception {};

    struct xInfiniteValue: std::exception {};

    struct xInvMatrixNotExist: std::exception {};

    Vector resolve(Matrix, Vector);

    double det(Matrix);

    Matrix inverse(Matrix);

}
