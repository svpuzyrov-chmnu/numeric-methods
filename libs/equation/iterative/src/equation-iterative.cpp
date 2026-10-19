#include "equation-chord.hpp"

namespace equation::iterative
{
    core::Result IterativeResolver::resolve(const std::function<double(const double&)>& equation, const core::Range& range) const
    {
        count_of_iterations_ = 0;

        if (equation(range.left()) == 0.0)
        {
            return core::Result{core::ResultType::DEFINED, range.left()};
        }

        if (equation(range.right()) == 0.0)
        {
            return core::Result{core::ResultType::DEFINED, range.right()};
        }

        if (!has_root_for(equation, range))
        {
            return core::Result{core::ResultType::UNDEFINED, 0.0};
        }

        auto iterative_step = [&](const double& x)
        {
            return x + factor_ * equation(x);
        };

        auto current_value = range.center();

        while (++count_of_iterations_, count_of_iterations_ <= max_iterations_)
        {
            const auto& nex_value = iterative_step(current_value);

            const auto& next_result = equation(nex_value);

            if (!std::isfinite(nex_value) || !std::isfinite(next_result))
            {
                return core::Result{core::ResultType::UNDEFINED, nex_value};
            }

            if (std::fabs(next_result) == 0.0)
            {
                return core::Result{core::ResultType::DEFINED, nex_value};
            }

            const auto& estimate_value = current_value + (nex_value - current_value) / 2;

            const auto& estimate_result = equation(estimate_value);

            if (std::fabs(nex_value - current_value) < tolerance_ &&
                std::fabs(next_result) < tolerance_ &&
                std::fabs(estimate_result) < tolerance_)
            {
                return core::Result{core::ResultType::DEFINED, estimate_value};
            }

            current_value = nex_value;
        }

        return core::Result{core::ResultType::UNDEFINED, 0.0};
    }
}
