#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "quadratures.hpp"

#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace numeric::quadratures;

TEST_CASE("Quadrature rules evaluate their documented nodes and weights")
{
    const Function linear = [](double x) { return x; };
    const Function cubic = [](double x) { return x * x * x; };

    SECTION("Left rectangle")
    {
        const auto result = LeftRectangleQuadrature{}.integrate(
            linear, {0.0, 1.0}, {2, 4});
        REQUIRE(result.value == Catch::Approx(0.25));
        REQUIRE(result.evaluations == 2);
        REQUIRE(result.nodes.size() == 2);
        REQUIRE(result.nodes[0].x == Catch::Approx(0.0));
        REQUIRE(result.nodes[0].weight == Catch::Approx(0.5));
        REQUIRE(result.nodes[1].x == Catch::Approx(0.5));
    }

    SECTION("Right rectangle")
    {
        const auto result = RightRectangleQuadrature{}.integrate(
            linear, {0.0, 1.0}, {2, 4});
        REQUIRE(result.value == Catch::Approx(0.75));
        REQUIRE(result.evaluations == 2);
        REQUIRE(result.nodes.size() == 2);
        REQUIRE(result.nodes[0].x == Catch::Approx(0.5));
        REQUIRE(result.nodes[1].x == Catch::Approx(1.0));
    }

    SECTION("Midpoint rectangle")
    {
        const auto result = MidpointRectangleQuadrature{}.integrate(
            linear, {0.0, 1.0}, {2, 4});
        REQUIRE(result.value == Catch::Approx(0.5));
        REQUIRE(result.evaluations == 2);
        REQUIRE(result.nodes.size() == 2);
        REQUIRE(result.nodes[0].x == Catch::Approx(0.25));
        REQUIRE(result.nodes[1].x == Catch::Approx(0.75));
    }

    SECTION("Trapezoid")
    {
        const auto result = TrapezoidQuadrature{}.integrate(
            linear, {0.0, 1.0}, {2, 4});
        REQUIRE(result.value == Catch::Approx(0.5));
        REQUIRE(result.evaluations == 3);
        REQUIRE(result.nodes.size() == 3);
        REQUIRE(result.nodes[0].weight == Catch::Approx(0.25));
        REQUIRE(result.nodes[1].weight == Catch::Approx(0.5));
        REQUIRE(result.nodes[2].weight == Catch::Approx(0.25));
    }

    SECTION("Simpson")
    {
        const auto result = SimpsonQuadrature{}.integrate(
            cubic, {0.0, 1.0}, {2, 4});
        REQUIRE(result.value == Catch::Approx(0.25));
        REQUIRE(result.evaluations == 3);
        REQUIRE(result.nodes.size() == 3);
        REQUIRE(result.nodes[0].weight == Catch::Approx(1.0 / 6.0));
        REQUIRE(result.nodes[1].weight == Catch::Approx(4.0 / 6.0));
        REQUIRE(result.nodes[2].weight == Catch::Approx(1.0 / 6.0));
    }

    SECTION("Gauss-Legendre")
    {
        const auto result = GaussLegendreQuadrature{}.integrate(
            cubic, {0.0, 1.0}, {1, 2});
        REQUIRE(result.value == Catch::Approx(0.25));
        REQUIRE(result.evaluations == 2);
        REQUIRE(result.nodes.size() == 2);
        REQUIRE(result.nodes[0].x == Catch::Approx(0.5 - 0.5 / std::sqrt(3.0)));
        REQUIRE(result.nodes[1].x == Catch::Approx(0.5 + 0.5 / std::sqrt(3.0)));
        REQUIRE(result.nodes[0].weight == Catch::Approx(0.5));
        REQUIRE(result.nodes[1].weight == Catch::Approx(0.5));
    }
}

TEST_CASE("Quadrature rules preserve signed interval orientation")
{
    const std::array<QuadratureType, 6> types{
        QuadratureType::LeftRectangle,
        QuadratureType::RightRectangle,
        QuadratureType::MidpointRectangle,
        QuadratureType::Trapezoid,
        QuadratureType::Simpson,
        QuadratureType::GaussLegendre
    };

    for (const auto type : types) {
        const auto result = QuadratureFactory::create(type)->integrate(
            [](double) { return 2.0; }, {2.0, 0.0}, {4, 4});
        REQUIRE(result.value == Catch::Approx(-4.0));
    }
}

TEST_CASE("Quadrature rules sample the exact upper endpoint")
{
    const std::array<QuadratureType, 3> types{
        QuadratureType::RightRectangle,
        QuadratureType::Trapezoid,
        QuadratureType::Simpson
    };

    for (const auto type : types) {
        const auto result = QuadratureFactory::create(type)->integrate(
            [](double x) { return x; }, {0.1, 0.3}, {4, 4});
        REQUIRE(result.nodes.back().x == 0.3);
    }
}

TEST_CASE("Gauss-Legendre canonical nodes are symmetric and normalized")
{
    for (std::size_t order = 1; order <= 64; ++order) {
        const auto nodes = GaussLegendreQuadrature::canonicalNodes(order);
        REQUIRE(nodes.size() == order);
        double weightSum = 0.0;
        for (std::size_t i = 0; i < order; ++i) {
            REQUIRE(std::isfinite(nodes[i].x));
            REQUIRE(std::isfinite(nodes[i].weight));
            REQUIRE(nodes[i].weight > 0.0);
            REQUIRE(nodes[i].x == Catch::Approx(-nodes[order - i - 1].x));
            REQUIRE(nodes[i].weight == Catch::Approx(nodes[order - i - 1].weight));
            weightSum += nodes[i].weight;
        }
        REQUIRE(weightSum == Catch::Approx(2.0).margin(1e-12));
    }
}

TEST_CASE("Quadrature validation rejects invalid options and intervals")
{
    SECTION("Empty function")
    {
        REQUIRE_THROWS_AS(
            LeftRectangleQuadrature{}.integrate({}, {0.0, 1.0}, {2, 4}),
            std::invalid_argument);
    }

    SECTION("Non-finite interval bounds")
    {
        REQUIRE_THROWS_AS(
            MidpointRectangleQuadrature{}.integrate(
                [](double x) { return x; },
                {0.0, std::numeric_limits<double>::infinity()}, {2, 4}),
            std::invalid_argument);
    }

    SECTION("Overflowing interval width")
    {
        REQUIRE_THROWS_AS(
            TrapezoidQuadrature{}.integrate(
                [](double x) { return x; },
                {-std::numeric_limits<double>::max(),
                 std::numeric_limits<double>::max()}, {2, 4}),
            std::invalid_argument);
    }

    SECTION("Zero subdivisions")
    {
        REQUIRE_THROWS_AS(
            LeftRectangleQuadrature{}.integrate(
                [](double x) { return x; }, {0.0, 1.0}, {0, 4}),
            std::invalid_argument);
    }

    SECTION("Odd Simpson subdivisions")
    {
        REQUIRE_THROWS_AS(
            SimpsonQuadrature{}.integrate(
                [](double x) { return x; }, {0.0, 1.0}, {3, 4}),
            std::invalid_argument);
    }

    SECTION("Invalid Gauss-Legendre order")
    {
        const Function function = [](double x) { return x; };
        REQUIRE_THROWS_AS(
            GaussLegendreQuadrature{}.integrate(function, {0.0, 1.0}, {2, 0}),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            GaussLegendreQuadrature{}.integrate(function, {0.0, 1.0}, {2, 65}),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            GaussLegendreQuadrature::canonicalNodes(0),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            GaussLegendreQuadrature::canonicalNodes(65),
            std::invalid_argument);
    }

    SECTION("Gauss-Legendre node count overflow")
    {
        REQUIRE_THROWS_AS(
            GaussLegendreQuadrature{}.integrate(
                [](double x) { return x; },
                {0.0, 1.0},
                {std::numeric_limits<std::size_t>::max(), 2}),
            std::invalid_argument);
    }
}

TEST_CASE("Quadrature evaluation rejects non-finite values and overflow")
{
    SECTION("Non-finite function value")
    {
        REQUIRE_THROWS_AS(
            MidpointRectangleQuadrature{}.integrate(
                [](double) { return std::numeric_limits<double>::quiet_NaN(); },
                {0.0, 1.0}, {2, 4}),
            std::domain_error);
    }

    SECTION("Overflowing individual contribution")
    {
        REQUIRE_THROWS_AS(
            LeftRectangleQuadrature{}.integrate(
                [](double) { return std::numeric_limits<double>::max(); },
                {0.0, 2.0}, {1, 4}),
            std::overflow_error);
    }

    SECTION("Overflowing accumulated result")
    {
        REQUIRE_THROWS_AS(
            LeftRectangleQuadrature{}.integrate(
                [](double) { return std::numeric_limits<double>::max(); },
                {0.0, 2.0}, {2, 4}),
            std::overflow_error);
    }
}

TEST_CASE("Quadrature factory creates every supported rule")
{
    const std::array<QuadratureType, 6> types{
        QuadratureType::LeftRectangle,
        QuadratureType::RightRectangle,
        QuadratureType::MidpointRectangle,
        QuadratureType::Trapezoid,
        QuadratureType::Simpson,
        QuadratureType::GaussLegendre
    };

    for (const auto type : types) {
        const auto quadrature = QuadratureFactory::create(type);
        REQUIRE(quadrature);
        REQUIRE(quadrature->type() == type);
    }
    REQUIRE_THROWS_AS(
        QuadratureFactory::create(static_cast<QuadratureType>(-1)),
        std::invalid_argument);
}
