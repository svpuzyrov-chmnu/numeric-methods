#pragma once

#include "matrix-all.hpp"

namespace linear_algebra::band
{
    class BandResolver
    {
    public:
        [[nodiscard]] static vector::Vector resolve(const matrix::band::BandMatrix<double>&, const vector::Vector&);
    };
}
