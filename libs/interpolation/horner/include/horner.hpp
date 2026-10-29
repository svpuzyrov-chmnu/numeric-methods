#pragma once

#include "polynom-core.hpp"

namespace polynomial::horner
{
    using polynomial::Coefficients;

    double evaluate(const Coefficients&, const double&);
}
