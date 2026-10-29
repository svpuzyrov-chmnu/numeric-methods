#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "lagrange.hpp"

#include <limits>
#include <stdexcept>

using interpolation::core::Points;
using interpolation::lagrange::evaluate;

TEST_CASE("Lagrange interpolation reproduces a linear function")
{
    const Points points{{-1.0, -1.0}, {0.0, 1.0}, {2.0, 5.0}};

    REQUIRE(evaluate(points, -2.0) == Catch::Approx(-3.0));
    REQUIRE(evaluate(points, 0.5) == Catch::Approx(2.0));
    REQUIRE(evaluate(points, 3.0) == Catch::Approx(7.0));
}

TEST_CASE("Lagrange interpolation reproduces a quadratic function")
{
    const Points points{{-1.0, 6.0}, {0.0, 3.0}, {2.0, 3.0}};

    REQUIRE(evaluate(points, -2.0) == Catch::Approx(11.0));
    REQUIRE(evaluate(points, 1.0) == Catch::Approx(2.0));
    REQUIRE(evaluate(points, 3.0) == Catch::Approx(6.0));
}

TEST_CASE("Lagrange interpolation returns the supplied values at nodes")
{
    const Points points{{-2.0, 4.0}, {0.0, -1.0}, {3.0, 8.0}};

    for (const auto& point : points)
    {
        REQUIRE(evaluate(points, point.x) == Catch::Approx(point.y));
    }
}

TEST_CASE("Lagrange interpolation handles a single data point")
{
    const Points points{{2.0, -5.0}};

    REQUIRE(evaluate(points, -10.0) == Catch::Approx(-5.0));
    REQUIRE(evaluate(points, 2.0) == Catch::Approx(-5.0));
}

TEST_CASE("Lagrange interpolation rejects invalid arguments and points")
{
    SECTION("empty points")
    {
        REQUIRE_THROWS_AS(evaluate(Points{}, 0.0), std::invalid_argument);
    }

    SECTION("non-finite argument")
    {
        const Points points{{0.0, 1.0}, {1.0, 2.0}};

        REQUIRE_THROWS_AS(
            evaluate(points, std::numeric_limits<double>::infinity()),
            std::invalid_argument
        );
    }

    SECTION("non-finite point coordinate")
    {
        const Points points{{0.0, 1.0}, {1.0, std::numeric_limits<double>::quiet_NaN()}};

        REQUIRE_THROWS_AS(evaluate(points, 0.5), std::invalid_argument);
    }

    SECTION("duplicate interpolation nodes")
    {
        const Points points{{1.0, 2.0}, {1.0, 3.0}};

        REQUIRE_THROWS_AS(evaluate(points, 0.5), std::invalid_argument);
    }
}
