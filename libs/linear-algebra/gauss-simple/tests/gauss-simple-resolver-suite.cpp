#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "gauss.hpp"

#include <limits>
#include <stdexcept>

using linear_algebra::gauss::simple::Matrix;
using linear_algebra::gauss::simple::Vector;
using linear_algebra::matrix::simple::operator*;
using linear_algebra::matrix::simple::operator-;
using linear_algebra::matrix::simple::make_matrix;
using linear_algebra::gauss::simple::resolve;

namespace
{
    Matrix make_pivoting_matrix(const size_t size)
    {
        return make_matrix(
            size,
            size,
            [size](const size_t& row, const size_t& col)
            {
                if (col == 0)
                {
                    return row == 0 ? 1.0 : (row == size - 1 ? 10.0 : 0.0);
                }
                return row == col ? 2.0 : 0.0;
            });
    }
}

static Matrix m_4x4{
    {-27.1489, -27.0561, 4.2419, -44.6195},
    {2.4671, -40.5327, 39.1517, -35.3451},
    {43.1613, -45.291, -16.3577, -10.1397},
    {44.6162, 3.4236, 19.3524, -24.0845},
};

static Matrix m5x5{
    {10.3, -22.5, 11.2, 6, 4.5},
    {-2.5, -4.1, -2.7, -2, 0.45},
    {1.2, 3.4, -0.5, 1, 2.17},
    {-0.8, 12.6, 3.1, 8.2, 4.55},
    {10.45, 19.11, -35.71, 14.08, -0.67}
};

TEST_CASE("Simple Partial Gauss resolver solves pivoting systems of the specific linear systems")
{
    SECTION("Resolve system size 4")
    {
        Vector rhs{2.5603, -10.1311, 8.5549, 18.4019};

        auto solution = resolve(m_4x4, rhs);

        auto back_rsh = m_4x4 * solution;

        REQUIRE(solution.size() == back_rsh.size());

        for (size_t i = 0; i < solution.size(); ++i)
        {
            REQUIRE(rhs[i] == Catch::Approx(back_rsh[i]));
        }
    }

    SECTION("Resolve system size 5")
    {
        Vector rhs{1.521, -0.0161, 2.4571, 13.7487, 1.0547};

        auto solution = resolve(m5x5, rhs);

        auto back_rsh = m5x5 * solution;

        REQUIRE(solution.size() == back_rsh.size());

        for (size_t i = 0; i < solution.size(); ++i)
        {
            REQUIRE(rhs[i] == Catch::Approx(back_rsh[i]));
        }
    }
}

TEST_CASE("Simple Partial Gauss resolver solves pivoting systems of sizes 2 through 5")
{
    const size_t size = GENERATE(size_t{2}, size_t{3}, size_t{4}, size_t{5});
    DYNAMIC_SECTION("Solves a " << size << "x" << size << " system")
    {
        auto coefficients = make_pivoting_matrix(size);

        Vector expected(size);

        for (size_t i = 0; i < size; ++i)
        {
            expected[i] = static_cast<double>(i + 1);
        }

        const auto rhs = coefficients * expected;

        const auto solution = resolve(coefficients, rhs);

        REQUIRE(solution.size() == size);
        for (size_t i = 0; i < size; ++i)
        {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
        }

        REQUIRE(coefficients.at(0).at(0) == 1.0);
        REQUIRE(coefficients.at(size - 1).at(0) == 10.0);
    }
}

TEST_CASE("Simple Partial Gauss resolver handles systems without a row swap")
{
    Matrix coefficients = {
        {4.0, 1.0, -1.0},
        {2.0, 5.0, 1.0},
        {1.0, -2.0, 6.0}
    };

    const Vector expected{2.0, -1.0, 3.0};

    const auto rhs = coefficients * expected;

    const auto solution = resolve(coefficients, rhs);

    for (size_t i = 0; i < expected.size(); ++i)
    {
        REQUIRE(solution[i] == Catch::Approx(expected[i]));
    }
}

TEST_CASE("Simple Partial Gauss resolver classifies singular systems")
{
    SECTION("Reports infinitely many solutions")
    {
        const Matrix coefficients = {{1.0, 1.0}, {2.0, 2.0}};

        REQUIRE_THROWS_AS(
            resolve(coefficients, Vector{2.0, 4.0}),
            linear_algebra::gauss::simple::xInfiniteSolution
        );
    }

    SECTION("Reports no solution")
    {
        const Matrix coefficients = {{1.0, 1.0}, {2.0, 2.0}};

        REQUIRE_THROWS_AS(
            resolve(coefficients, Vector{2.0, 5.0}),
            linear_algebra::gauss::simple::xNoSolution
        );
    }
}

TEST_CASE("Simple Partial Gauss resolver validates dimensions and finite diagonal inputs")
{
    SECTION("Rejects non-square matrices")
    {
        const Matrix coefficients = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};

        REQUIRE_THROWS_AS(
            resolve(coefficients, Vector{1.0, 2.0}),
            linear_algebra::gauss::simple::xNonSquareMatrix
        );
    }

    SECTION("Rejects mismatched right-hand-side length")
    {
        const Matrix coefficients(2, Vector(2));

        REQUIRE_THROWS_AS(
            resolve(coefficients, Vector{1.0}),
            linear_algebra::gauss::simple::xMismatchRightSideSize
        );
    }

    SECTION("Rejects non-finite diagonal coefficients")
    {
        const Matrix coefficients{
            {1.5, 2},
            {2.0, std::numeric_limits<double>::infinity()}
        };

        REQUIRE_THROWS_AS(
            resolve(coefficients, Vector{1.0, -1.3}),
            linear_algebra::gauss::simple::xInfiniteValue
        );
    }

    SECTION("Rejects non-finite non-diagonal coefficients")
    {
        const Matrix coefficients{
            {1.5, 2},
            {std::numeric_limits<double>::infinity(), 2.0}
        };

        REQUIRE_THROWS_AS(
            resolve(coefficients, Vector{1.0, -1.3}),
            linear_algebra::gauss::simple::xInfiniteValue
        );
    }

    SECTION("Rejects non-finite right-hand-side values for 1x1 matrice")
    {
        const Matrix coefficients{{1.0}};

        REQUIRE_THROWS_AS(
            resolve(coefficients, Vector{std::numeric_limits<double>::infinity()}),
            linear_algebra::gauss::simple::xInfiniteValue
        );
    }

    SECTION("Rejects non-finite right-hand-side values for 2x2 matrice")
    {
        const Matrix coefficients{{1.0, 3.0}, {-4.2, 0.0}};

        const Vector rhs {0.5, std::numeric_limits<double>::infinity()};

        REQUIRE_THROWS_AS(
            resolve(coefficients, rhs),
            linear_algebra::gauss::simple::xInfiniteValue
        );
    }

    SECTION("Returns an empty solution for an empty system")
    {
        const Matrix coefficients {};

        auto result = resolve(coefficients, Vector(0));

        REQUIRE(result.size() == 0);
    }
}
