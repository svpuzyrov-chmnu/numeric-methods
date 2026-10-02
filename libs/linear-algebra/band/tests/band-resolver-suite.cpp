#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "band-resolver.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

using linear_algebra::band::BandResolver;
using linear_algebra::matrix::band::BandMatrix;
using linear_algebra::vector::Vector;

namespace
{
    Vector make_rhs(const BandMatrix<double>& matrix, const Vector& expected)
    {
        return matrix * expected;
    }
}

TEST_CASE("Band resolver solves systems with different bandwidths")
{
    SECTION("Solves a matrix with unequal lower and upper bandwidths")
    {
        BandMatrix<double> matrix(5, 2, 1);
        for (size_t i = 0; i < matrix.size(); ++i)
        {
            matrix(i, i) = 8.0 + i;
            if (i > 0)
            {
                matrix(i, i - 1) = -1.0;
            }
            if (i > 1)
            {
                matrix(i, i - 2) = 0.5;
            }
            if (i + 1 < matrix.size())
            {
                matrix(i, i + 1) = 1.5;
            }
        }

        const Vector expected{1.0, -2.0, 3.0, 0.5, -1.0};
        const auto solution = BandResolver::resolve(matrix, make_rhs(matrix, expected));

        REQUIRE(solution.size() == expected.size());
        for (size_t i = 0; i < expected.size(); ++i)
        {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
        }
    }

    SECTION("Solves a matrix initialized from five diagonal bands")
    {
        const BandMatrix<double> matrix(5, std::vector<std::vector<double>>{
            {0.0, 0.5, 0.5, 0.5, 0.5},
            {-1.0, -1.0, -1.0, -1.0, 0.0},
            {8.0, 9.0, 10.0, 11.0, 12.0},
            {0.0, 1.5, 1.5, 1.5, 1.5},
            {0.25, 0.25, 0.25, 0.0, 0.0}
        });

        const Vector expected{2.0, -1.0, 0.5, 3.0, -2.0};
        const auto solution = BandResolver::resolve(matrix, make_rhs(matrix, expected));

        for (size_t i = 0; i < expected.size(); ++i)
        {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
        }
    }

    SECTION("Uses row pivoting when a diagonal pivot is zero")
    {
        BandMatrix<double> matrix(3, 1, 1);
        matrix(0, 1) = 1.0;
        matrix(1, 0) = 2.0;
        matrix(1, 1) = 3.0;
        matrix(1, 2) = 1.0;
        matrix(2, 1) = 1.0;
        matrix(2, 2) = 4.0;

        const Vector expected{1.0, 2.0, -1.0};
        const auto solution = BandResolver::resolve(matrix, make_rhs(matrix, expected));

        for (size_t i = 0; i < expected.size(); ++i)
        {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
        }
    }
}

TEST_CASE("Band resolver validates systems")
{
    SECTION("Rejects a right-hand side with the wrong size")
    {
        const BandMatrix<double> matrix(2, 1, 1);

        REQUIRE_THROWS_AS(BandResolver::resolve(matrix, Vector{1.0}), std::invalid_argument);
    }

    SECTION("Returns an empty vector for an empty system")
    {
        const BandMatrix<double> matrix(0, 1, 1);

        REQUIRE(BandResolver::resolve(matrix, Vector{}).size() == 0);
    }

    SECTION("Rejects a singular matrix")
    {
        BandMatrix<double> matrix(2, 1, 1);
        matrix(0, 0) = 1.0;
        matrix(0, 1) = 2.0;
        matrix(1, 0) = 2.0;
        matrix(1, 1) = 4.0;

        REQUIRE_THROWS_AS(
            BandResolver::resolve(matrix, Vector{3.0, 6.0}),
            std::runtime_error
        );
    }

    SECTION("Rejects non-finite coefficients and right-hand-side values")
    {
        BandMatrix<double> matrix(1, 0, 0);
        matrix(0, 0) = std::numeric_limits<double>::infinity();
        REQUIRE_THROWS_AS(
            BandResolver::resolve(matrix, Vector{1.0}),
            std::invalid_argument
        );

        matrix(0, 0) = 1.0;
        REQUIRE_THROWS_AS(
            BandResolver::resolve(matrix, Vector{std::numeric_limits<double>::infinity()}),
            std::invalid_argument
        );
    }
}
