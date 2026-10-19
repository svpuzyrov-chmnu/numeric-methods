#include "equation-bisect.hpp"

namespace equation::bisect
{

    core::Result BisectResolver::resolve(const std::function<double(const double&)>& equation,
                                            const core::Range& range) const
    {
        count_of_iterations_ = 0;

        const auto& left_value = equation(range.left());

        if (left_value == 0.0)
        {
            return core::Result{core::ResultType::DEFINED, range.left()};
        }

        const auto& right_value = equation(range.right());

        if (right_value == 0.0)
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
            const auto& center = range1.center();

            const auto& center_value = equation(center);

            if (center_value == 0.0)
            {
                return core::Result{core::ResultType::DEFINED, center};
            }

            const auto& left_half = range1.left_center();

            if (has_root_for(equation, left_half))
            {
                range1 = left_half;
            }
            else
            {
                range1 = range1.right_center();
            }

            if (range1.length_is_lq(tolerance_))
            {
                return core::Result{core::ResultType::DEFINED, range1.center()};
            }
        }

        return core::Result{core::ResultType::UNDEFINED, 0.0};
    }
}
