#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "non-linear-core.hpp"

namespace nonlinear::core::utils
{
    double norm(const Vector&);

    bool is_finite(const Vector&);

    void validate(const NonlinearSystem&, const IterationOptions&);

    Vector evaluate(const NonlinearSystem&, const Vector&);

    Matrix jacobian(const NonlinearSystem&, const Vector&, const double&);
}
