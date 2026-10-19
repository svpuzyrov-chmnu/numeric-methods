#pragma once

#include <vector.hpp>

namespace equation::core
{
    struct InputData
    {
        std::string description;
        std::function<double(const double&)> equation;
        double factor;
        Range range;
        double tolerance;
        double expected;
    };
}
