#pragma once

#include "matrix-all.hpp"

namespace linear_algebra::tridiagonal
{
    class TridiagonalResolver
    {
    public:
        static vector::Vector resolve(const matrix::band::BandMatrix<double>&, const vector::Vector&);
    };
}
