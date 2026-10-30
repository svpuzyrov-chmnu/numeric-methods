#include "free-boundary-generator.hpp"

namespace interpolation::spline::core::boundary {

    BoundaryEquation FreeBoundaryEquationGenerator::generate(const BoundaryContext &context) const {
        return {
            1.0,
            -1.0,
            0.0
        };
    }
};
