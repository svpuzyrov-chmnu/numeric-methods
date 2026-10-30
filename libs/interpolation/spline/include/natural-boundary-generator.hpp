#pragma once

#include "boundary-generator.hpp"

namespace interpolation::spline::core::boundary {

    class NaturalBoundaryEquationGenerator final
        : public IBoundaryEquationGenerator {
    public:
        BoundaryEquation generate(const BoundaryContext&) const override;
    };
}
