#include "quadrature-detail.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

namespace numeric::quadratures::detail {
    void validate(const Function &f, Interval interval, const QuadratureOptions &options,
                  QuadratureType type) {

        if (!f)
            throw std::invalid_argument("Function must not be empty");

        if (!std::isfinite(interval.lower) || !std::isfinite(interval.upper))
            throw std::invalid_argument("Interval bounds must be finite");

        if (!std::isfinite(interval.upper - interval.lower))
            throw std::invalid_argument("Interval width must be finite");

        if (options.subdivisions == 0)
            throw std::invalid_argument("Subdivisions must be positive");

        if (type == QuadratureType::Simpson && options.subdivisions % 2 != 0)
            throw std::invalid_argument("Simpson rule requires even subdivisions");

        if (type == QuadratureType::GaussLegendre &&
            (options.gaussOrder == 0 || options.gaussOrder > 64))
            throw std::invalid_argument("Gauss-Legendre order must be in [1,64]");

        if (type == QuadratureType::GaussLegendre &&
            options.subdivisions > std::numeric_limits<std::size_t>::max() / options.gaussOrder)
            throw std::invalid_argument("Too many quadrature nodes");
    }

    void evaluate(const Function &f, double x, QuadratureResult &result, double weight) {
        if (!std::isfinite(x))
            throw std::domain_error("Quadrature node must be finite");

        const double y = f(x);

        if (!std::isfinite(y))
            throw std::domain_error("Function returned non-finite value");

        if (!std::isfinite(weight))
            throw std::overflow_error("Quadrature weight is non-finite");

        const double contribution = y * weight;

        if (!std::isfinite(contribution))
            throw std::overflow_error("Quadrature contribution is non-finite");

        const double value = result.value + contribution;

        if (!std::isfinite(value))
            throw std::overflow_error("Quadrature result is non-finite");

        result.value = value;

        ++result.evaluations;

        result.nodes.push_back({x, weight});
    }
} // namespace numeric::quadratures::detail
