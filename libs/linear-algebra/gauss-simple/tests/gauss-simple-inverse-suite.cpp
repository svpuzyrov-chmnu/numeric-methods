#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "gauss.hpp"

#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

using linear_algebra::gauss::simple::Matrix;
using linear_algebra::gauss::simple::Vector;
using linear_algebra::gauss::simple::inverse;
using linear_algebra::matrix::simple::operator*;

namespace
{
    const Matrix m1_4x4{
        {-27.1489, -27.0561, 4.2419, -44.6195},
        {2.4671, -40.5327, 39.1517, -35.3451},
        {43.1613, -45.291, -16.3577, -10.1397},
        {44.6162, 3.4236, 19.3524, -24.0845},
    };

    const Matrix m2_4x4{
        {5, 4, 0, -7},
        {-4, 2, -1, 6},
        {1, 10, 3, 2},
        {3, 7, 5, -3},
    };

    const Matrix m5x5{
        {10.3, -22.5, 11.2, 6, 4.5},
        {-2.5, -4.1, -2.7, -2, 0.45},
        {1.2, 3.4, -0.5, 1, 2.17},
        {-0.8, 12.6, 3.1, 8.2, 4.55},
        {10.45, 19.11, -35.71, 14.08, -0.67}
    };
}

TEST_CASE("Simple Partial resolver solves pivoting systems to determine inverse matrix")
{
    SECTION("Simple Partial resolver solves pivoting systems to determine inverse double matrix of size 4")
    {
        auto inv = inverse(m1_4x4);

        auto identity = inv * m1_4x4;

        for (size_t i = 0; i < identity.size(); ++i)
        {
            for (size_t j = 0; j < identity.size(); ++j)
            {
                auto expected = (i == j) ? 1.0 : 0.0;
                REQUIRE_THAT(expected, Catch::Matchers::WithinAbs(identity.at(i).at(j), 1e-9));
            }
        }
    }

    SECTION("Simple Partial resolver solves pivoting systems to determine inverse int matrix of size 4")
    {
        auto inv2 = inverse(m2_4x4);

        auto identity = inv2 * m2_4x4;

        for (size_t i = 0; i < identity.size(); ++i)
        {
            for (size_t j = 0; j < identity.size(); ++j)
            {
                auto expected = (i == j) ? 1.0 : 0.0;
                REQUIRE_THAT(expected, Catch::Matchers::WithinAbs(identity.at(i).at(j), 1e-9));
            }
        }
    }

    SECTION("Simple Partial resolver solves pivoting systems to determine inverse matrix of size 5")
    {
        auto inv3 = inverse(m5x5);

        auto identity = inv3 * m5x5;

        for (size_t i = 0; i < identity.size(); ++i)
        {
            for (size_t j = 0; j < identity.size(); ++j)
            {
                auto expected = (i == j) ? 1.0 : 0.0;
                REQUIRE_THAT(expected, Catch::Matchers::WithinAbs(identity.at(i).at(j), 1e-9));
            }
        }
    }
}
