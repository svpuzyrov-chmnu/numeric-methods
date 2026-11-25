#pragma once


#include "quadrature-types.hpp"

namespace numeric::quadratures::simple
{
    double left_rect(const QuadratureOptions&);

    double right_rect(const QuadratureOptions&);

    double middle_rect(const QuadratureOptions&);

    double trapezoidal(const QuadratureOptions&);
    
    double simpson(const QuadratureOptions&);

}