#pragma once

#include <functional>
#include "range.hpp"
#include "result.hpp"
#include "input-data.hpp"

namespace equation::core
{
    class IEquationResolver
    {
    protected:
        static bool has_root_for(const std::function<double(const double&)>&, const Range&);
        const double tolerance_;
        const int max_iterations_;

        mutable int count_of_iterations_;

    public:
        static const int default_max_iterations;
        static const double default_tolerance;

        IEquationResolver(const double& tolerance, const int& max_iterations)
            : tolerance_(tolerance)
              , max_iterations_(max_iterations)
              , count_of_iterations_(0)
        {}

        IEquationResolver(const double& tolerance)
            : IEquationResolver(tolerance, default_max_iterations)
        {}

        IEquationResolver()
            : IEquationResolver(default_tolerance, default_max_iterations)
        {}

        virtual ~IEquationResolver() = default;

        [[nodiscard]] virtual Result resolve(const std::function<double(const double&)>&, const Range&) const = 0;

        [[nodiscard]] int count_of_iterations() const
        {
            return count_of_iterations_;
        }
    };
}
