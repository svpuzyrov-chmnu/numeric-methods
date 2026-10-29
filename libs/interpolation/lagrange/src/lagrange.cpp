#include "lagrange.hpp"
#include <cmath>
#include <stdexcept>

namespace interpolation::lagrange {
    double evaluate(const core::Points &points, const double &x) {
        if (const std::size_t n = points.size(); n == 0) {
            throw std::invalid_argument(
                "Interpolation points are empty"
            );
        }

        if (!std::isfinite(x)) {
            throw std::invalid_argument(
                "Argument must be finite"
            );
        }

        core::validate(points);

        double L = 0.0;

        const auto &size = points.size();

        for (std::size_t i = 0; i < size; ++i) {
            double l = 1.0;

            auto [xi, yi] = points.at(i);

            for (std::size_t j = 0; j < size; ++j) {
                if (i != j) {
                    auto [xj, yj] = points.at(j);

                    l *= (x - xj) / (xi - xj);
                }
            }

            L += yi * l;
        }

        return L;
    }
}
