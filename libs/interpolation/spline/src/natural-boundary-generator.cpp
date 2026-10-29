#include "natural-boundary-generator.hpp"

namespace interpolation::spline::core::boundary {

    BoundaryEquation NaturalBoundaryEquationGenerator::generate(const BoundaryContext &context) const {
        return {
            1.0,
            0.0,
            0.0
        };
    }
};
