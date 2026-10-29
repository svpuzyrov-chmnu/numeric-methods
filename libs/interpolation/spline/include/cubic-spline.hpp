#pragma once

#include <memory>
#include <matrix.hpp>

#include "spline-core.hpp"
#include "boundary-generator.hpp"
#include "band-matrix.hpp"
#include "vector.hpp"

namespace interpolation::spline::cubic {
    using core::Point;
    using core::Points;
    using core::Node;
    using core::Nodes;
    using core::Segment;

    using linear_algebra::matrix::band::BandMatrix;
    using linear_algebra::vector::Vector;

    class CubicSpline {
    public:
        using CubicSegment = Segment<3>;
        using Segments = std::vector<CubicSegment>;
        using Generator = std::shared_ptr<const core::boundary::IBoundaryEquationGenerator>;

        explicit CubicSpline(const Points &, Generator left, Generator right);

        [[nodiscard]]
        double evaluate(double) const;

        [[nodiscard]]
        double derivative(double x, std::size_t order = 1) const;

        [[nodiscard]] const Nodes &nodes() const {
            return nodes_;
        }

        [[nodiscard]] const Segments &segments() const {
            return segments_;
        }

    private:

        void build();

        [[nodiscard]] std::size_t interval(double) const;

        Generator bcl_generator_;
        Generator bcr_generator_;
        Nodes nodes_;
        Segments segments_;
    };
}
