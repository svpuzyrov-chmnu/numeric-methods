#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "partial-gauss.hpp"
#include "matrix-all.hpp"

#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

using linear_algebra::gauss::PartialGaussInverseMatrixResolver;
using linear_algebra::matrix::RectangleMatrix;
using linear_algebra::vector::Vector;

namespace {
    const PartialGaussInverseMatrixResolver resolver;

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

TEST_CASE("Partial Gauss resolver solves pivoting systems to determine inverse matrix") {

    SECTION("Partial Gauss resolver solves pivoting systems to determine inverse matrix of size 4") {
        RectangleMatrix source = raw_m_4x4;

        auto inverse = resolver.inverse(source);

        auto identity = inverse * source ;

        for (size_t i = 0; i < identity.rows(); ++i) {
            for (size_t j = 0; j < identity.cols(); ++j) {
                auto expected = (i == j) ? 1.0 : 0.0;
                REQUIRE_THAT(expected, Catch::Matchers::WithinAbs(identity(i, j), 1e-9));
            }
        }
    }

    SECTION("Partial Gauss resolver solves pivoting systems to determine inverse matrix of size 5") {
        RectangleMatrix source = raw_m5x5;

        auto inverse = resolver.inverse(source);

        auto identity = inverse * source ;

        for (size_t i = 0; i < identity.rows(); ++i) {
            for (size_t j = 0; j < identity.cols(); ++j) {
                auto expected = (i == j) ? 1.0 : 0.0;
                REQUIRE_THAT(expected, Catch::Matchers::WithinAbs(identity(i, j), 1e-9));
            }
        }
    }
}
