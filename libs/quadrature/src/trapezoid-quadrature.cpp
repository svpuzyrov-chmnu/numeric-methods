#include "trapezoid-quadrature.hpp"
#include "quadrature-detail.hpp"
#include <cmath>
#include <stdexcept>

namespace numeric::quadratures {
    QuadratureResult TrapezoidQuadrature::integrate(const Function &f,
                                                    Interval in,
                                                    const QuadratureOptions &opt) const {
        detail::validate(f, in, opt, type());

        QuadratureResult r;

        const double h = (in.upper - in.lower) / opt.subdivisions;

        for (std::size_t i = 0; i < opt.subdivisions; ++i) {
            const double weight = i == 0 ? h / 2.0 : h;
            detail::evaluate(f, in.lower + i * h, r, weight);
        }

        detail::evaluate(f, in.upper, r, h / 2.0);

        return r;
    }
} // namespace numeric::quadratures
