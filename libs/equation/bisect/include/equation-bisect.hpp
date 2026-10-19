#pragma once

#include "equation-core.hpp"

namespace equation::bisect
{
    class BisectResolver : public core::IEquationResolver
    {
    public:
        BisectResolver(const double& tolerance, const int& max_iterations)
            :IEquationResolver(tolerance, max_iterations)
        {}

        BisectResolver(const double& tolerance)
            :BisectResolver(tolerance, default_max_iterations)
        {}

        ~BisectResolver() override = default;

        [[nodiscard]] core::Result resolve(const std::function<double(const double&)>&, const core::Range&) const override;
    };
}
