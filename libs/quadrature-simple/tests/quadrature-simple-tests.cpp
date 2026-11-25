#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "quadrature-simple.hpp"

#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace numeric::quadratures::simple;

TEST_CASE("Simple quadrature methods (L, R, M, T, S)")
{
    const Function linear = [](double x) { return x; };
    const Function cubic = [](double x) { return x * x * x; };

    SECTION("Left rectangle")
    {
        QuadratureOptions options {0.0, 1.0, 2, linear};
        const auto result = left_rect(options);

        REQUIRE(result == Catch::Approx(0.25));
    }

    SECTION("Right rectangle")
    {
        QuadratureOptions options {0.0, 1.0, 2, linear};

        const auto result = right_rect(options);
        REQUIRE(result == Catch::Approx(0.75));
    }

    SECTION("Midpoint rectangle")
    {
        QuadratureOptions options {0.0, 1.0, 2, linear};

        const auto result = middle_rect(options);

        REQUIRE(result == Catch::Approx(0.5));
    }

    SECTION("Trapezoid")
    {
        QuadratureOptions options {0.0, 1.0, 2, linear};

        const auto result = trapezoidal(options);

        REQUIRE(result == Catch::Approx(0.5));
    }

    SECTION("Simpson")
    {
        QuadratureOptions options {0.0, 1.0, 2, cubic};

        const auto result = simpson(options);

        REQUIRE(result == Catch::Approx(0.25));
    }
}
