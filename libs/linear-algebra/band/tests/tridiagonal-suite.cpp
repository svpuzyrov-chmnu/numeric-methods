#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "matrix-all.hpp"
#include "tridiagonal-resolver.hpp"

#include <stdexcept>

using linear_algebra::tridiagonal::TridiagonalResolver;
using linear_algebra::matrix::band::BandMatrix;
using linear_algebra::matrix::band::operator*;
using linear_algebra::vector::Vector;

namespace
{
    BandMatrix<double> make_tridiagonal(
        const Vector& lower,
        const Vector& diagonal,
        const Vector& upper)
    {
        const auto size = diagonal.size();
        BandMatrix<double> matrix(size, 1, 1);

        for (size_t i = 0; i < size; ++i)
        {
            matrix(i, i) = diagonal[i];
            if (i > 0)
            {
                matrix(i, i - 1) = lower[i - 1];
            }
            if (i + 1 < size)
            {
                matrix(i, i + 1) = upper[i];
            }
        }

        return matrix;
    }
}

TEST_CASE("Tridiagonal resolver solves systems")
{
    SECTION("Solves a 4x4 system 1")
    {
        const auto matrix = make_tridiagonal(
            Vector{-1.0, -1.0, -1.0},
            Vector{4.0, 4.0, 4.0, 4.0},
            Vector{-1.0, -1.0, -1.0}
        );
        const Vector expected{1.0, 2.0, 3.0, 4.0};
        const Vector rhs{2.0, 4.0, 6.0, 13.0};

        const auto solution = TridiagonalResolver::resolve(matrix, rhs);

        REQUIRE(solution.size() == expected.size());
        for (size_t i = 0; i < expected.size(); ++i)
        {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
        }
    }

    SECTION("Solves a 4x4 system 2")
    {
        const auto matrix = make_tridiagonal(
            Vector{34.21, -21.39, 10.02},
            Vector{34.3, -39.0, -35.97, -24.73},
            Vector{15.76, -18.59, 33.46}
        );

        const Vector rhs{35.12, -34.51, -42.06, 14.1};

        const auto solution = TridiagonalResolver::resolve(matrix, rhs);

        std::cout << "Solves a 4x4 system 2" << std::endl;
        std::cout << solution << std::endl;
        const Vector check = matrix * solution;

        REQUIRE(solution.size() == check.size());
        for (size_t i = 0; i < check.size(); ++i)
        {
            REQUIRE(rhs[i] == Catch::Approx(check[i]));
        }
    }

    SECTION("Solves a single-equation system")
    {
        const auto matrix = make_tridiagonal(Vector{}, Vector{2.0}, Vector{});

        const auto solution = TridiagonalResolver::resolve(matrix, Vector{6.0});

        REQUIRE(solution.size() == 1);
        REQUIRE(solution[0] == Catch::Approx(3.0));
    }

    SECTION("Returns an empty solution for an empty system")
    {
        const auto matrix = make_tridiagonal(Vector{}, Vector{}, Vector{});

        REQUIRE(TridiagonalResolver::resolve(matrix, Vector{}).size() == 0);
    }
}

TEST_CASE("Tridiagonal resolver validates input dimensions and bandwidth")
{
    SECTION("Rejects a right-hand side of the wrong size")
    {
        const auto matrix = make_tridiagonal(
            Vector{1.0}, Vector{4.0, 4.0}, Vector{1.0}
        );

        REQUIRE_THROWS_AS(
            TridiagonalResolver::resolve(matrix, Vector{1.0}),
            std::invalid_argument
        );
    }

    SECTION("Rejects matrices with a wider band")
    {
        const BandMatrix<double> matrix(3, 1, 2);

        REQUIRE_THROWS_AS(
            TridiagonalResolver::resolve(matrix, Vector{1.0, 1.0, 1.0}),
            std::invalid_argument
        );
    }
}

TEST_CASE("Tridiagonal resolver reports zero pivots")
{
    SECTION("Detects a zero pivot in the first row")
    {
        const auto matrix = make_tridiagonal(
            Vector{1.0}, Vector{0.0, 2.0}, Vector{1.0}
        );

        REQUIRE_THROWS_AS(
            TridiagonalResolver::resolve(matrix, Vector{1.0, 1.0}),
            std::runtime_error
        );
    }

    SECTION("Detects a zero pivot during forward elimination")
    {
        const auto matrix = make_tridiagonal(
            Vector{1.0, 1.0},
            Vector{1.0, 1.0, 1.0},
            Vector{1.0, 1.0}
        );

        REQUIRE_THROWS_AS(
            TridiagonalResolver::resolve(matrix, Vector{1.0, 1.0, 1.0}),
            std::runtime_error
        );
    }
}
