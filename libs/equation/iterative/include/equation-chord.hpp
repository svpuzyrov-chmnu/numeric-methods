#pragma once

#include "equation-core.hpp"

namespace equation::iterative
{
    class IterativeResolver : public core::IEquationResolver
    {
        const double factor_;

    public:
        IterativeResolver(const double& factor, const double& tolerance, const int& max_iterations)
            : IEquationResolver(tolerance, max_iterations)
              , factor_(factor)
        {}

        IterativeResolver(const double& factor, const double& tolerance)
            : IterativeResolver(factor, tolerance, default_max_iterations)
        {}

        IterativeResolver(const double& factor)
            : IterativeResolver(factor, default_tolerance, default_max_iterations)
        {}

        ~IterativeResolver() override = default;

        [[nodiscard]] core::Result
        resolve(const std::function<double(const double&)>&, const core::Range&) const override;
    };
}
