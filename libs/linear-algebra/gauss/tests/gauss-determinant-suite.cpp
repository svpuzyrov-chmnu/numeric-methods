#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "partial-gauss.hpp"
#include "matrix-all.hpp"

using linear_algebra::gauss::PartialGaussDeterminantResolver;
using linear_algebra::matrix::RectangleMatrix;
using linear_algebra::vector::Vector;

namespace {
    const PartialGaussDeterminantResolver resolver;

    double raw_m_4x4[][4] = {
        {-27.1489, -27.0561, 4.2419, -44.6195},
        {2.4671, -40.5327, 39.1517, -35.3451},
        {43.1613, -45.291, -16.3577, -10.1397},
        {44.6162, 3.4236, 19.3524, -24.0845},
    };

    double raw_m5x5[][5] = {
        {10.3, -22.5, 11.2, 6, 4.5},
        {-2.5, -4.1, -2.7, -2, 0.45},
        {1.2, 3.4, -0.5, 1, 2.17},
        {-0.8, 12.6, 3.1, 8.2, 4.55},
        {10.45, 19.11, -35.71, 14.08, -0.67}
    };
}

TEST_CASE("Partial Gauss resolver solves determinant of matrix") {
    SECTION("Partial Gauss resolver solves determinant of matrix size 4") {
        RectangleMatrix coefficients = raw_m_4x4;

        constexpr auto expected = 6.33724e+6;

        const auto actual = resolver.determinant(coefficients);

        REQUIRE(expected == Catch::Approx(actual));
    }

    SECTION("Partial Gauss resolver solves determinant of matrix size 5") {
        RectangleMatrix coefficients = raw_m5x5;

        constexpr auto expected = -1.08959e+5;

        const auto actual = resolver.determinant(coefficients);

        REQUIRE(expected == Catch::Approx(actual));
    }
}
