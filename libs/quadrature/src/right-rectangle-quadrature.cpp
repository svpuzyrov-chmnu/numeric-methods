#include "right-rectangle-quadrature.hpp"
#include "quadrature-detail.hpp"
#include <cmath>
#include <stdexcept>

namespace numeric::quadratures {
    QuadratureResult RightRectangleQuadrature::integrate(const Function &f, Interval in,
                                                         const QuadratureOptions &opt) const {
        detail::validate(f, in, opt, type());

        QuadratureResult r;

        const double h = (in.upper - in.lower) / opt.subdivisions;

        for (std::size_t i = 0; i < opt.subdivisions; ++i) {
            const double x = i + 1 == opt.subdivisions
                ? in.upper
                : in.lower + (i + 1) * h;
            detail::evaluate(f, x, r, h);
        }

        return r;
    }
} // namespace numeric::quadratures
