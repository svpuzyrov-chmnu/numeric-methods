#include "gauss-legendre-quadrature.hpp"
#include "quadrature-detail.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace numeric::quadratures {
    namespace {
        // Canonical Gauss-Legendre nodes and weights, Newton iteration on P_n.
        std::pair<double, double> legendreAndDerivative(std::size_t n, double x) {
            double prev = 1.0, curr = x;

            if (n == 1) {
                return {x, 1.0};
            }

            for (std::size_t k = 2; k <= n; ++k) {
                const double next = ((2.0 * k - 1.0) * x * curr - (k - 1.0) * prev) / k;
                prev = curr;
                curr = next;
            }

            const double derivative = n * (x * curr - prev) / (x * x - 1.0);

            return {curr, derivative};
        }
    } // namespace

    std::vector<QuadratureNode> GaussLegendreQuadrature::canonicalNodes(std::size_t n) {
        if (n == 0 || n > 64) throw std::invalid_argument("Gauss-Legendre order must be in [1,64]");

        std::vector<QuadratureNode> nodes(n);

        const double pi = std::acos(-1.0);

        for (std::size_t i = 0; i < (n + 1) / 2; ++i) {
            double x = std::cos(pi * (i + 0.75) / (n + 0.5));

            double dp = 0.0;

            bool converged = false;

            for (int iter = 0; iter < 100; ++iter) {
                const auto [p, derivative] = legendreAndDerivative(n, x);

                dp = derivative;

                const double delta = p / dp;

                x -= delta;

                if (std::abs(delta) < 1e-15) {
                    converged = true;
                    break;
                }
            }
            if (!converged) {
                throw std::runtime_error("Gauss-Legendre root did not converge");
            }

            if (n % 2 == 1 && i == n / 2) {
                x = 0.0;
            }

            dp = legendreAndDerivative(n, x).second;

            const double w = 2.0 / ((1.0 - x * x) * dp * dp);

            nodes[i] = {-x, w};

            nodes[n - i - 1] = {x, w};
        }
        return nodes;
    }

    QuadratureResult GaussLegendreQuadrature::integrate(const Function &f, Interval in,
                                                        const QuadratureOptions &opt) const {
        detail::validate(f, in, opt, type());

        QuadratureResult r;

        const auto nodes = canonicalNodes(opt.gaussOrder);

        const double h = (in.upper - in.lower) / opt.subdivisions;

        for (std::size_t k = 0; k < opt.subdivisions; ++k) {
            const double a = in.lower + k * h;
            const double b = (k + 1 == opt.subdivisions) ? in.upper : in.lower + (k + 1) * h;
            const double center = a / 2.0 + b / 2.0;
            const double half = (b - a) / 2.0;
            for (const auto &[x, weight]: nodes)
                detail::evaluate(f, center + half * x, r, half * weight);
        }
        return r;
    }
} // namespace numeric::quadratures
