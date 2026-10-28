#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "horner.hpp"

#include <cmath>
#include <string>
#include <vector>

using polynomial::horner::Coefficients;
using polynomial::horner::evaluate;

namespace
{
    struct RootCase
    {
        std::string name;
        Coefficients coefficients;
        double x;
        double expected;
    };
}

TEST_CASE("Evaluate polynom values for the given value using Horner algorithm")
{
    const std::vector<RootCase> cases{
        {"5", {5.0}, 3.0, 5.0},
        {"2x + 3", {3.0, 2.0}, 4.0, 11.0},
        {"-3x + 7", {7.0, -3.0}, 2.0, 1.0},
        {"x^2 - 5x + 6", {6.0, -5.0, 1.0}, 2.0, 0.0},
        {"2x^2 + 3x - 1", {-1.0, 3.0, 2.0}, 2.0, 13.0},
        {"x^3 - 2x^2 + 3x - 4", {-4.0, 3.0, -2.0, 1.0}, 2.0, 2.0},
        {"x^3 - 6x^2 + 11x - 6", {-6.0, 11.0, -6.0, 1.0}, 3.0, 0.0},
        {"x^4 - 3x^2 + 2", {2.0, 0.0, -3.0, 0.0, 1.0}, 2.0, 6.0},
        {"x^5 - 2x^3 + x - 1", {-1.0, 1.0, 0.0, -2.0, 0.0, 1.0}, 2.0, 17.0},
        {"2x^3 - x^2 + 4x + 5", {5.0, 4.0, -1.0, 2.0}, -2.0, -23.0},
        {"3x^3 + 2x^2 - x + 7", {7.0, -1.0, 2.0, 3.0}, 0.0, 7.0},
        {"4x^2 - 2x + 1", {1.0, -2.0, 4.0}, 0.5, 1.0},
        {"0.5x^2 + 1.5x - 2", {-2.0, 1.5, 0.5}, 2.0, 3.0},
        {"x^4 + 2", {2.0, 0.0, 0.0, 0.0, 1.0}, 3.0, 83.0},
        {"x^6 - x^4 + x^2 - 1", {-1.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0}, 2.0, 51.0}
    };

    for (const auto&[name, coefficients, x, expected] : cases)
    {
        DYNAMIC_SECTION("Evaluate " << name << "in point: " << x)
        {
            const auto result = evaluate(coefficients, x);

            REQUIRE(result == Catch::Approx(expected).margin(1e-8));
        }
    }
}
