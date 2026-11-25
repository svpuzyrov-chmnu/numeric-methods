#include "quadrature-simple.hpp"
#include <stdexcept>

namespace numeric::quadratures::simple {

    double left_rect(const QuadratureOptions& options)
    {
        auto S = 0.0;

        if (options.is_singular())
        {
            return S;
        }

        for (auto i = 0; i < options.subdivisions(); ++i)
        {
            S += options.next_value_at(i);
        }

        return options.step() * S;
    }

    double right_rect(const QuadratureOptions& options)
    {
        auto S = 0.0;

        if (options.is_singular())
        {
            return S;
        }

        for (auto i = 1; i <= options.subdivisions(); ++i)
        {
            S += options.next_value_at(i);
        }

        return options.step() * S;
    }

    double middle_rect(const QuadratureOptions& options)
    {
        auto S = 0.0;

        if (options.is_singular())
        {
            return S;
        }

        for (auto i = 0; i < options.subdivisions(); ++i)
        {
            S += options.next_value_at([&]
            {
                return options.from() + (i + 0.5) * options.step();
            });
        }

        return options.step() * S;
    }

    double trapezoidal(const QuadratureOptions& options)
    {
        if (options.is_singular())
        {
            return 0.0;
        }

        auto S = (options.from_value() + options.to_value()) / 2;

        for (auto i = 1; i < options.subdivisions(); ++i)
        {
            S += options.next_value_at(i);
        }

        return options.step() * S;
    }

    double simpson(const QuadratureOptions& options)
    {
        if (options.subdivisions() % 2 != 0)
            throw std::runtime_error("Simpson method requires even subdivisions");

        if (options.is_singular())
        {
            return 0.0;
        }

        auto S = options.from_value() + options.to_value();

        for (auto i = 1; i < options.subdivisions(); ++i)
        {
            const auto& factor = i % 2 == 0 ? 2 : 4;

            S += factor * options.next_value_at(i);
        }

        return options.step() * S / 3.0;
    }
} 