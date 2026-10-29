#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "least-squares.hpp"

#include <limits>
#include <stdexcept>

using interpolation::core::Points;
using interpolation::list_squares::LeastSquares;

TEST_CASE("Least squares fits a line to the input points")
{
    Points points{{-1.0, -3.0}, {0.0, -1.0}, {1.0, 1.0}, {2.0, 3.0}};
    const LeastSquares leastSquares(points);

    const auto polynomial = leastSquares.approx(1);

    REQUIRE(polynomial.degree() == 1);
    REQUIRE(polynomial.coefficients()[0] == Catch::Approx(-1.0));
    REQUIRE(polynomial.coefficients()[1] == Catch::Approx(2.0));
    REQUIRE(leastSquares.squaredError(polynomial) == Catch::Approx(0.0).margin(1e-10));
}

TEST_CASE("Least squares fits a quadratic polynomial")
{
    Points points{{-1.0, 6.0}, {0.0, 2.0}, {1.0, 0.0}, {2.0, 0.0}};
    const LeastSquares leastSquares(points);

    const auto polynomial = leastSquares.approx(2);
    const auto coefficients = polynomial.coefficients();

    REQUIRE(polynomial.degree() == 2);
    REQUIRE(coefficients[0] == Catch::Approx(2.0));
    REQUIRE(coefficients[1] == Catch::Approx(-3.0));
    REQUIRE(coefficients[2] == Catch::Approx(1.0));
}

TEST_CASE("Least squares uses the mean for a degree-zero approximation")
{
    Points points{{0.0, 1.0}, {1.0, 3.0}, {2.0, 5.0}};
    const LeastSquares leastSquares(points);

    const auto polynomial = leastSquares.approx(0);

    REQUIRE(polynomial.degree() == 0);
    REQUIRE(polynomial(10.0) == Catch::Approx(3.0));
    REQUIRE(leastSquares.squaredError(polynomial) == Catch::Approx(8.0));
}

TEST_CASE("Least squares minimizes squared error for noisy linear data")
{
    Points points{{-2.0, 1.0}, {-1.0, 2.0}, {0.0, 2.0}, {1.0, 4.0}, {2.0, 6.0}};
    const LeastSquares leastSquares(points);

    const auto polynomial = leastSquares.approx(1);
    const auto coefficients = polynomial.coefficients();

    REQUIRE(coefficients[0] == Catch::Approx(3.0));
    REQUIRE(coefficients[1] == Catch::Approx(1.2));
    REQUIRE(leastSquares.squaredError(polynomial) == Catch::Approx(1.6));
}

TEST_CASE("Least squares rejects invalid input and unsupported degrees")
{
    SECTION("empty dataset")
    {
        Points points;

        REQUIRE_THROWS_AS(LeastSquares(points), std::invalid_argument);
    }

    SECTION("non-finite point coordinate")
    {
        Points points{{std::numeric_limits<double>::infinity(), 1.0}};

        REQUIRE_THROWS_AS(LeastSquares(points), std::invalid_argument);
    }

    SECTION("degree must be less than the number of points")
    {
        Points points{{0.0, 1.0}, {1.0, 2.0}};
        const LeastSquares leastSquares(points);

        REQUIRE_THROWS_AS(leastSquares.approx(2), std::invalid_argument);
    }

    SECTION("duplicate point coordinates are rejected")
    {
        Points points{{1.0, 2.0}, {1.0, 3.0}, {1.0, 4.0}};

        REQUIRE_THROWS_AS(LeastSquares(points), std::invalid_argument);
    }
}
