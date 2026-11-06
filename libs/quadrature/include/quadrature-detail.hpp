#pragma once
#include "quadrature-types.hpp"

namespace numeric::quadratures::detail {
    void validate(const Function &,
                  Interval,
                  const QuadratureOptions &,
                  QuadratureType);

    void evaluate(const Function &,
                  double,
                  QuadratureResult &,
                  double);
} // namespace numeric::quadratures::detail
