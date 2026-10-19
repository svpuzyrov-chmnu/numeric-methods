#pragma once

#include "equation-core.hpp"

namespace equation::chord
{
    class ChordResolver : public core::IEquationResolver
    {

    public:
        ChordResolver(const double& tolerance, const int& max_iterations)
            : IEquationResolver(tolerance, max_iterations)
        {}

        ChordResolver(const double& tolerance)
            : ChordResolver(tolerance, default_max_iterations)
        {}

        ChordResolver()
            : ChordResolver(default_tolerance, default_max_iterations)
        {}

        ~ChordResolver() override = default;

        [[nodiscard]] core::Result resolve(const std::function<double(const double&)>&, const core::Range&) const override;
    };
}
