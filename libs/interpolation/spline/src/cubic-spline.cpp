#include "cubic-spline.hpp"
#include "tridiagonal-resolver.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace interpolation::spline::cubic {
    static void validate_points(const Points &points) {
        if (points.size() < 2) {
            throw std::invalid_argument(
                "At least two interpolation points are required"
            );
        }

        for (std::size_t i = 0; i < points.size(); ++i) {
            const auto &point = points[i];

            if (!std::isfinite(point.x) ||
                !std::isfinite(point.y)) {
                throw std::invalid_argument(
                    "Spline points must be finite"
                );
            }

            if (i > 0 && point.x <= points[i - 1].x) {
                throw std::invalid_argument(
                    "Spline points must be strictly increasing"
                );
            }
        }
    }


    CubicSpline::CubicSpline(const Points &points, Generator left, Generator right)
        : bcl_generator_(std::move(left)),
          bcr_generator_(std::move(right)) {
        validate_points(points);

        if (!bcl_generator_ || !bcr_generator_) {
            throw std::invalid_argument(
                "Boundary generators must not be null"
            );
        }

        nodes_ = core::create_from(points);

        build();
    }

    double CubicSpline::evaluate(double x) const {
        return derivative(x, 0);
    }

    double CubicSpline::derivative(double x, std::size_t order) const {
        const auto i = interval(x);
        const auto &segment = segments_[i];

        const double t = x - nodes_[i].point().x;

        const double a = segment.get_coefficient(0);
        const double b = segment.get_coefficient(1);
        const double c = segment.get_coefficient(2);
        const double d = segment.get_coefficient(3);

        switch (order) {
            case 0:
                return ((d * t + c) * t + b) * t + a;

            case 1:
                return (3.0 * d * t + 2.0 * c) * t + b;

            case 2:
                return 6.0 * d * t + 2.0 * c;

            case 3:
                return 6.0 * d;

            default:
                return 0.0;
        }
    }

    void CubicSpline::build() {
        using core::boundary::BoundaryContext;
        using core::boundary::BoundarySide;
        using core::boundary::BoundaryEquation;
        using TL_Resolver = linear_algebra::tridiagonal::TridiagonalResolver;

        const std::size_t n_count = nodes_.size();
        const std::size_t up_index = n_count - 1;

        // 1. Length of segments and split differences
        std::vector<double> h(up_index);
        std::vector<double> delta(up_index);

        for (std::size_t i = 0; i < up_index; ++i) {
            const auto [left_x, left_y] = nodes_[i].point();
            const auto [right_x, right_y] = nodes_[i + 1].point();

            h[i] = right_x - left_x;
            delta[i] = (right_y - left_y) / h[i];
        }

        // 2. Create 3-diagonal matrix
        BandMatrix<double> matrix(n_count, 1, 1);
        Vector rhs(n_count);

        // 3. Internal continuously equations  S'(x)
        for (std::size_t i = 1; i < up_index; ++i) {
            matrix(i, i - 1) = h[i - 1];

            matrix(i, i) = 2.0 * (h[i - 1] + h[i]);

            matrix(i, i + 1) = h[i];

            rhs[i] = 3.0 * (delta[i] - delta[i - 1]);
        }

        // 4. Border condition equations
        {
            const auto [diagonal, adjacent, rhs_] =
                    bcl_generator_->generate({
                        BoundarySide::Left,
                        h.front(),
                        delta.front()
                    });
            // 5. Add left equation
            matrix(0, 0) = diagonal;
            matrix(0, 1) = adjacent;
            rhs[0] = rhs_;
        }

        {
            const auto [diagonal, adjacent, rhs_] =
                    bcr_generator_->generate({
                        BoundarySide::Right,
                        h.back(),
                        delta.back()
                    });


            // 6. Add right equation
            matrix(up_index, up_index - 1) = adjacent;
            matrix(up_index, up_index) = diagonal;

            rhs[up_index] = rhs_;
        }

        const auto c = TL_Resolver::resolve(
            matrix,
            rhs
        );

        Segments segments;
        segments.reserve(up_index);

        for (std::size_t i = 0; i < up_index; ++i) {
            CubicSegment::Coefficients coefficients;

            coefficients[0] = nodes_[i].point().y;

            coefficients[1] = delta[i] -
                              h[i] * (2.0 * c[i] + c[i + 1]) / 3.0;

            coefficients[2] = c[i];

            coefficients[3] = (c[i + 1] - c[i]) / (3.0 * h[i]);

            CubicSegment segment(
                coefficients,
                nodes_[i],
                nodes_[i + 1]
            );

            segments.emplace_back(segment);
        }

        segments_ = std::move(segments);
    }

    std::size_t CubicSpline::interval(double x) const {
        if (!std::isfinite(x)) {
            throw std::invalid_argument(
                "Argument must be finite"
            );
        }

        const auto first = nodes_.front().point().x;
        const auto last = nodes_.back().point().x;

        if (x < first || x > last) {
            throw std::out_of_range(
                "Argument outside interpolation domain"
            );
        }

        if (x == last) {
            return nodes_.size() - 2;
        }

        const auto it = std::upper_bound(
            nodes_.begin(),
            nodes_.end(),
            x,
            [](const double &value, const Node &node) {
                return value < node.point().x;
            }
        );

        return static_cast<std::size_t>(
            std::distance(nodes_.begin(), it) - 1
        );
    }
}
