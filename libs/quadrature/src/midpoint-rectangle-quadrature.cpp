#include "midpoint-rectangle-quadrature.hpp"
#include "quadrature-detail.hpp"
#include <cmath>
#include <stdexcept>

namespace numeric::quadratures {
    QuadratureResult MidpointRectangleQuadrature::integrate(const Function &f,
                                                            Interval in,
                                                            const QuadratureOptions &opt) const {
        detail::validate(f, in, opt, type());

        QuadratureResult r;

        const double h = (in.upper - in.lower) / opt.subdivisions;

        for (std::size_t i = 0; i < opt.subdivisions; ++i) {
            detail::evaluate(f, in.lower + (i + 0.5) * h, r, h);
        }

        return r;
    }
} // namespace numeric::quadratures
