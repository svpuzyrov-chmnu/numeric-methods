#include "non-linear-utils.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>

namespace nonlinear::core::utils
{
    double norm(const Vector& v)
    {
        double scale = 0.0;
        double sum = 1.0;

        for (const auto& value : v)
        {
            if (!std::isfinite(value))
            {
                return std::numeric_limits<double>::infinity();
            }

            const double a = std::abs(value);

            if (a == 0.0)
            {
                continue;
            }

            if (scale < a)
            {
                const double ratio = scale / a;
                sum = 1.0 + sum * ratio * ratio;
                scale = a;
            }
            else
            {
                const double ratio = a / scale;
                sum += ratio * ratio;
            }
        }

        return scale == 0.0 ? 0.0 : scale * std::sqrt(sum);
    }

    bool is_finite(const Vector& v)
    {
        return std::all_of(
            v.begin(),
            v.end(),
            [](const auto& value)
            {
                return std::isfinite(value);
            });
    }

    void validate(const NonlinearSystem& system, const IterationOptions& options)
    {
        if (system.dimension == 0)
        {
            throw std::invalid_argument(
                "System dimension must be positive");
        }

        if (!system.function)
        {
            throw std::invalid_argument(
                "Nonlinear function is not defined");
        }

        if (options.max_iterations == 0)
        {
            throw std::invalid_argument(
                "Maximum iterations must be positive");
        }

        if (!std::isfinite(options.tolerance) ||
            options.tolerance <= 0.0)
        {
            throw std::invalid_argument(
                "Tolerance must be finite and positive");
        }

        if (!std::isfinite(options.jacobian_step) ||
            options.jacobian_step <= 0.0)
        {
            throw std::invalid_argument(
                "Jacobian step must be finite and positive");
        }

        if (!std::isfinite(options.min_damping) ||
            options.min_damping <= 0.0 ||
            options.min_damping > 1.0)
        {
            throw std::invalid_argument(
                "Minimum damping must be in (0, 1]");
        }
    }

    Vector evaluate(const NonlinearSystem& system, const Vector& x)
    {
        if (x.size() != system.dimension)
        {
            throw std::invalid_argument(
                "Incorrect solution vector dimension");
        }

        Vector values = system.function(x);

        if (values.size() != system.dimension)
        {
            throw std::invalid_argument(
                "Number of equations must equal system dimension");
        }

        return values;
    }

    Matrix jacobian(const NonlinearSystem& system, const Vector& x, const double& step)
    {
        const std::size_t size = system.dimension;

        Matrix J(size, Vector(size, 0.0));

        for (std::size_t col = 0; col < size; ++col)
        {
            Vector plus = x;
            Vector minus = x;

            const double h = step * std::max(1.0, std::abs(x.at(col)));

            plus.at(col) += h;
            minus.at(col) -= h;

            const Vector f_plus = evaluate(system, plus);
            const Vector f_minus = evaluate(system, minus);

            if (!is_finite(f_plus) || !is_finite(f_minus))
            {
                throw std::domain_error(
                    "Non-finite function value while calculating Jacobian"
                    );
            }

            for (std::size_t row = 0; row < size; ++row)
            {
                const auto& value = (f_plus.at(row) - f_minus.at(row)) / (2.0 * h);

                if (!std::isfinite(value))
                {
                    throw std::domain_error(
                        "Non-finite Jacobian coefficient");
                }

                J.at(row).at(col) = value;
            }
        }

        return J;
    }
}
