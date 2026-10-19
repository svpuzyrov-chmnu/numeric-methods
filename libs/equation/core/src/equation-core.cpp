#include "equation-core.hpp"

namespace equation::core
{
    const int IEquationResolver::default_max_iterations = 10000;

    const double IEquationResolver::default_tolerance = 1e-9;

    bool IEquationResolver::has_root_for(const std::function<double(const double&)>& equation, const Range& range)
    {
        const auto& value_range = range.map([&](auto x) { return std::signbit(equation(x)); });

        return value_range.left() != value_range.right();
    }
}
