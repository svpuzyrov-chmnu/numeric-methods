#include "clamp-boundary-generator.hpp"
#include <cmath>
#include <stdexcept>

namespace interpolation::spline::core::boundary {

    ClampBoundaryEquationGenerator::ClampBoundaryEquationGenerator(double slope)
            : slope_(slope) {
        if (!std::isfinite(slope_)) {
            throw std::invalid_argument(
                "Boundary slope must be finite"
            );
        }
    }

    BoundaryEquation ClampBoundaryEquationGenerator::generate(const BoundaryContext &context) const {
        const double rhs =
             context.side == BoundarySide::Left
                 ? 3.0 * (context.delta - slope_)
                 : 3.0 * (slope_ - context.delta);

        return {
            2.0 * context.h,
            context.h,
            rhs
        };
    }
};
