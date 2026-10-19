#include "equation-chord.hpp"

#include <algorithm>
#include <cmath>

namespace equation::chord
{
    core::Result ChordResolver::resolve(const std::function<double(const double&)>& equation,
                                        const core::Range& range) const
    {
        count_of_iterations_ = 0;

        auto value_range = range.map(equation);

        if (value_range.left() == 0.0)
        {
            return core::Result{core::ResultType::DEFINED, range.left()};
        }

        if (value_range.right() == 0.0)
        {
            return core::Result{core::ResultType::DEFINED, range.right()};
        }

        if (!has_root_for(equation, range))
        {
            return core::Result{core::ResultType::UNDEFINED, 0.0};
        }

        auto range1 = range;

        while (++count_of_iterations_, count_of_iterations_ <= max_iterations_)
        {
            const auto scale = std::max(std::fabs(value_range.left()), std::fabs(value_range.right()));
            const auto left_value = value_range.left() / scale;
            const auto right_value = value_range.right() / scale;
            const auto denominator = right_value - left_value;

            if (std::fabs(denominator) == 0.0)
            {
                return core::Result{core::ResultType::UNDEFINED, 0.0};
            }

            const auto x = range1.left() - left_value * range1.length() / denominator;

            if (range1.is_border(x))
            {
                return core::Result{core::ResultType::DEFINED, x};
            }

            const auto& current_value = equation(x);

            if (std::fabs(current_value) == 0.0)
            {
                return core::Result{core::ResultType::DEFINED, x};
            }

            if (std::signbit(current_value) == std::signbit(value_range.left()))
            {
                range1 = core::Range{x, range1.right()};
                value_range = core::Range{current_value, value_range.right()};
            }
            else
            {
                range1 = core::Range{range1.left(), x};
                value_range = core::Range{value_range.left(), current_value};
            }

            if (range1.length_is_lq(tolerance_))
            {
                return core::Result{core::ResultType::DEFINED, range1.center()};
            }
        }

        return core::Result{core::ResultType::UNDEFINED, 0.0};
    }
}
