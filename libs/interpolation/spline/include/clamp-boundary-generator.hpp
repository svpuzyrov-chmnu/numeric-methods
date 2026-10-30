#pragma once

#include "boundary-generator.hpp"

namespace interpolation::spline::core::boundary {

    class ClampBoundaryEquationGenerator final
        : public IBoundaryEquationGenerator {
    public:
        explicit ClampBoundaryEquationGenerator(double slope);
        BoundaryEquation  generate(const BoundaryContext&) const override;
    private:
        double slope_;
    };
}
