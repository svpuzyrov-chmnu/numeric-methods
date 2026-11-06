#include "simpson-quadrature.hpp"
#include "quadrature-detail.hpp"
#include <cmath>
#include <stdexcept>

namespace numeric::quadratures {
    QuadratureResult SimpsonQuadrature::integrate(const Function &f,
                                                  Interval in,
                                                  const QuadratureOptions &opt) const {
        detail::validate(f, in, opt, type());

        QuadratureResult r;

        const double h = (in.upper - in.lower) / opt.subdivisions;

        for (std::size_t i = 0; i < opt.subdivisions; ++i) {
            const double factor = i == 0 ? 1.0 : (i % 2 ? 4.0 : 2.0);

            detail::evaluate(f, in.lower + i * h, r, factor * (h / 3.0));
        }

        detail::evaluate(f, in.upper, r, h / 3.0);

        return r;
    }
} // namespace numeric::quadratures
