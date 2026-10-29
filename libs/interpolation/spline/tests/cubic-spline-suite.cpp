#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "clamp-boundary-generator.hpp"
#include "cubic-spline.hpp"
#include "free-boundary-generator.hpp"
#include "natural-boundary-generator.hpp"

#include <limits>
#include <memory>
#include <stdexcept>

using interpolation::core::Points;
using interpolation::spline::core::boundary::ClampBoundaryEquationGenerator;
using interpolation::spline::core::boundary::FreeBoundaryEquationGenerator;
using interpolation::spline::core::boundary::NaturalBoundaryEquationGenerator;
using interpolation::spline::cubic::CubicSpline;

namespace {
    using Generator = CubicSpline::Generator;

    Generator natural_boundary()
    {
        return std::make_shared<NaturalBoundaryEquationGenerator>();
    }

    Generator clamped_boundary(double slope)
    {
        return std::make_shared<ClampBoundaryEquationGenerator>(slope);
    }

    Generator free_boundary()
    {
        return std::make_shared<FreeBoundaryEquationGenerator>();
    }
}

TEST_CASE("Clamped cubic spline reproduces a cubic polynomial and its derivatives")
{
    const auto polynomial = [](double x) {
        return x * x * x - 2.0 * x * x + 3.0 * x - 4.0;
    };
    const auto first_derivative = [](double x) {
        return 3.0 * x * x - 4.0 * x + 3.0;
    };
    const auto second_derivative = [](double x) {
        return 6.0 * x - 4.0;
    };
    const Points points{
        {-1.0, polynomial(-1.0)},
        {0.5, polynomial(0.5)},
        {2.0, polynomial(2.0)},
        {4.0, polynomial(4.0)}
    };
    const CubicSpline spline(
        points,
        clamped_boundary(first_derivative(points.front().x)),
        clamped_boundary(first_derivative(points.back().x))
    );

    for (const double x : {-1.0, -0.25, 0.5, 1.25, 2.0, 3.0, 4.0}) {
        REQUIRE(spline.evaluate(x) == Catch::Approx(polynomial(x)));
        REQUIRE(spline.derivative(x, 1) == Catch::Approx(first_derivative(x)));
        REQUIRE(spline.derivative(x, 2) == Catch::Approx(second_derivative(x)));
        REQUIRE(spline.derivative(x, 3) == Catch::Approx(6.0));
    }
}

TEST_CASE("Natural spline interpolates every node and has zero endpoint curvature")
{
    const Points points{{0.0, 1.0}, {0.5, -1.0}, {2.0, 3.0}, {3.0, 2.0}};
    const CubicSpline spline(points, natural_boundary(), natural_boundary());

    for (const auto& point : points) {
        REQUIRE(spline.evaluate(point.x) == Catch::Approx(point.y));
    }

    REQUIRE(spline.derivative(points.front().x, 2) == Catch::Approx(0.0));
    REQUIRE(spline.derivative(points.back().x, 2) == Catch::Approx(0.0));
}

TEST_CASE("Cubic spline first and second derivatives are continuous at interior nodes")
{
    const Points points{{-2.0, 3.0}, {-0.5, -2.0}, {1.0, 4.0}, {3.5, 1.0}};
    const CubicSpline spline(points, natural_boundary(), natural_boundary());
    const auto& nodes = spline.nodes();
    const auto& segments = spline.segments();

    for (std::size_t i = 1; i + 1 < nodes.size(); ++i) {
        const double h = nodes[i].point().x - nodes[i - 1].point().x;
        const double left_b = segments[i - 1].get_coefficient(1);
        const double left_c = segments[i - 1].get_coefficient(2);
        const double left_d = segments[i - 1].get_coefficient(3);
        const double left_first = (3.0 * left_d * h + 2.0 * left_c) * h + left_b;
        const double left_second = 6.0 * left_d * h + 2.0 * left_c;

        REQUIRE(left_first == Catch::Approx(spline.derivative(nodes[i].point().x, 1)));
        REQUIRE(left_second == Catch::Approx(spline.derivative(nodes[i].point().x, 2)));
    }
}

TEST_CASE("Clamped boundary generators impose the requested endpoint slopes")
{
    const Points points{{0.0, 2.0}, {1.0, -1.0}, {3.0, 4.0}};
    const CubicSpline spline(
        points,
        clamped_boundary(-2.5),
        clamped_boundary(1.25)
    );

    REQUIRE(spline.derivative(points.front().x, 1) == Catch::Approx(-2.5));
    REQUIRE(spline.derivative(points.back().x, 1) == Catch::Approx(1.25));
}

TEST_CASE("Free boundary generators make the end segments linear")
{
    const Points points{{0.0, 1.0}, {1.0, 4.0}, {2.5, -2.0}};
    const CubicSpline spline(points, free_boundary(), free_boundary());

    REQUIRE(spline.derivative(points.front().x, 3) == Catch::Approx(0.0));
    REQUIRE(spline.derivative(points[1].x, 3) == Catch::Approx(0.0));
    REQUIRE(spline.derivative(points.back().x, 3) == Catch::Approx(0.0));
}

TEST_CASE("Cubic spline rejects invalid points")
{
    SECTION("fewer than two points")
    {
        REQUIRE_THROWS_AS(
            CubicSpline(Points{}, natural_boundary(), natural_boundary()),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            CubicSpline(Points{{1.0, 2.0}}, natural_boundary(), natural_boundary()),
            std::invalid_argument
        );
    }

    SECTION("non-finite coordinates")
    {
        REQUIRE_THROWS_AS(
            CubicSpline(
                Points{{0.0, 1.0}, {1.0, std::numeric_limits<double>::infinity()}},
                natural_boundary(),
                natural_boundary()
            ),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            CubicSpline(
                Points{{0.0, std::numeric_limits<double>::quiet_NaN()}, {1.0, 2.0}},
                natural_boundary(),
                natural_boundary()
            ),
            std::invalid_argument
        );
    }

    SECTION("nodes are not strictly increasing")
    {
        REQUIRE_THROWS_AS(
            CubicSpline(Points{{1.0, 2.0}, {1.0, 3.0}}, natural_boundary(), natural_boundary()),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            CubicSpline(Points{{2.0, 2.0}, {1.0, 3.0}}, natural_boundary(), natural_boundary()),
            std::invalid_argument
        );
    }
}

TEST_CASE("Cubic spline rejects null boundary generators")
{
    const Points points{{0.0, 0.0}, {1.0, 1.0}};
    const Generator null_generator;

    REQUIRE_THROWS_AS(
        CubicSpline(points, null_generator, natural_boundary()),
        std::invalid_argument
    );
    REQUIRE_THROWS_AS(
        CubicSpline(points, natural_boundary(), null_generator),
        std::invalid_argument
    );
}

TEST_CASE("Cubic spline rejects non-finite and out-of-domain query arguments")
{
    const CubicSpline spline(
        Points{{0.0, 0.0}, {1.0, 1.0}},
        natural_boundary(),
        natural_boundary()
    );

    REQUIRE_THROWS_AS(
        spline.evaluate(std::numeric_limits<double>::quiet_NaN()),
        std::invalid_argument
    );
    REQUIRE_THROWS_AS(
        spline.derivative(std::numeric_limits<double>::infinity()),
        std::invalid_argument
    );
    REQUIRE_THROWS_AS(spline.evaluate(-0.1), std::out_of_range);
    REQUIRE_THROWS_AS(spline.derivative(1.1), std::out_of_range);
}

TEST_CASE("Derivative orders above the cubic degree are zero")
{
    const CubicSpline spline(
        Points{{0.0, 2.0}, {1.0, 5.0}},
        natural_boundary(),
        natural_boundary()
    );

    REQUIRE(spline.derivative(0.5, 4) == 0.0);
}
