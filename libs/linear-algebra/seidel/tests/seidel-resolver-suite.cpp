#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "seidel.hpp"
#include "matrix-all.hpp"

#include <limits>
#include <stdexcept>

using linear_algebra::seidel::SeidelResolver;
using linear_algebra::matrix::RectangleMatrix;
using linear_algebra::vector::Vector;

namespace
{
    const SeidelResolver resolver;

    RectangleMatrix make_pivoting_matrix(const size_t size)
    {
        return RectangleMatrix {
            size,
            size,
            [](const size_t&, const size_t&) { return true; },
            [size](const size_t& row, const size_t& col)
            {
                if (col == 0)
                {
                    return row == 0 ? 1.0 : (row == size - 1 ? 10.0 : 0.0);
                }
                return row == col ? 2.0 : 0.0;
            }
        };
    }

    Vector make_right_hand_side(const RectangleMatrix& coefficients, const Vector& expected)
    {
        return coefficients * expected;
    }

    double raw_m_4x4[][4] = {
        {10.0, -1.0, 2.0, 0.0},
        {-1.0, 11.0, -1.0, 3.0},
        {2.0, -1.0, 10.0, -1.0},
        {0.0, 3.0, -1.0, 8.0},
    };

    double raw_m5x5[][5] = {
        {12.0, 1.0, -1.0, 0.0, 2.0},
        {1.0, 13.0, 2.0, -1.0, 0.0},
        {-1.0, 2.0, 14.0, 1.0, -2.0},
        {0.0, -1.0, 1.0, 15.0, 1.0},
        {2.0, 0.0, -2.0, 1.0, 16.0}
    };
}

TEST_CASE("Seidel resolver solves diagonally dominant systems")
{
    SECTION("Resolve system size 4")
    {
        RectangleMatrix coefficients = raw_m_4x4;

        Vector expected{1.0, 2.0, -1.0, 1.0};
        const auto rhs = make_right_hand_side(coefficients, expected);

        auto solution = resolver.resolve(coefficients, rhs);

        auto back_rsh = make_right_hand_side(coefficients, solution);

        REQUIRE(solution.size() == back_rsh.size());

        for (size_t i = 0; i < solution.size(); ++i)
        {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
            REQUIRE(rhs[i] == Catch::Approx(back_rsh[i]));
        }
    }

    SECTION("Resolve system size 5")
    {
        RectangleMatrix coefficients = raw_m5x5;

        Vector expected{1.0, -1.0, 2.0, 0.5, -2.0};
        const auto rhs = make_right_hand_side(coefficients, expected);

        auto solution = resolver.resolve(coefficients, rhs);

        auto back_rsh = make_right_hand_side(coefficients, solution);

        REQUIRE(solution.size() == back_rsh.size());

        for (size_t i = 0; i < solution.size(); ++i)
        {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
            REQUIRE(rhs[i] == Catch::Approx(back_rsh[i]));
        }
    }
}

TEST_CASE("Seidel resolver solves pivoting systems of sizes 2 through 5")
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
        const auto rhs = make_right_hand_side(coefficients, expected);

        const auto solution = resolver.resolve(coefficients, rhs);

        REQUIRE(solution.size() == size);
        for (size_t i = 0; i < size; ++i)
        {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
        }

        REQUIRE(coefficients(0, 0) == 1.0);
        REQUIRE(coefficients(size - 1, 0) == 10.0);
    }
}

TEST_CASE("Seidel resolver handles systems without a row swap")
{
    constexpr double data[3][3] = {
        {4.0, 1.0, -1.0},
        {2.0, 5.0, 1.0},
        {1.0, -2.0, 6.0}
    };

    const RectangleMatrix coefficients(data);

    const Vector expected{2.0, -1.0, 3.0};

    const auto right_hand_side = make_right_hand_side(coefficients, expected);

    const auto solution = resolver.resolve(coefficients, right_hand_side);

    for (size_t i = 0; i < expected.size(); ++i)
    {
        REQUIRE(solution[i] == Catch::Approx(expected[i]));
    }
}

TEST_CASE("Seidel resolver handles singular systems")
{
    SECTION("Returns a solution for a consistent singular system")
    {
        constexpr double data[2][2] = {{1.0, 1.0}, {2.0, 2.0}};
        const RectangleMatrix coefficients(data);

        const auto solution = resolver.resolve(coefficients, Vector{2.0, 4.0});
        const auto back_rhs = coefficients * solution;
        REQUIRE(back_rhs[0] == Catch::Approx(2.0));
        REQUIRE(back_rhs[1] == Catch::Approx(4.0));
    }

    SECTION("Reports failure to converge for an inconsistent singular system")
    {
        constexpr double data[2][2] = {{1.0, 1.0}, {2.0, 2.0}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{2.0, 5.0}),
            std::runtime_error
        );
    }
}

TEST_CASE("Seidel resolver applies matrix and vector transformations")
{
    constexpr double data[2][2] = {{4.0, 2.0}, {1.0, 5.0}};
    const RectangleMatrix coefficients(data);
    const Vector right_hand_side{8.0, 11.0};

    const auto solution = resolver.resolve(coefficients, right_hand_side);

    REQUIRE(solution[0] == Catch::Approx(1.0));
    REQUIRE(solution[1] == Catch::Approx(2.0));
    REQUIRE(coefficients(0, 0) == 4.0);
}

TEST_CASE("Seidel resolver rejects invalid inputs to its transformations")
{
    SECTION("Rejects a matrix with a zero diagonal")
    {
        constexpr double data[2][2] = {{1.0, 2.0}, {3.0, 0.0}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(resolver.resolve(coefficients, Vector{1.0, 1.0}), std::invalid_argument);
    }

    SECTION("Rejects an overflowing scaled right-hand side")
    {
        constexpr double data[1][1] = {{1e-10}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{std::numeric_limits<double>::max()}),
            std::invalid_argument
        );
    }
}

TEST_CASE("Seidel resolver validates dimensions and finite diagonal inputs")
{
    SECTION("Rejects non-square matrices")
    {
        constexpr double data[2][3] = {
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0}
        };
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{1.0, 2.0}),
            std::invalid_argument
        );
    }

    SECTION("Rejects mismatched right-hand-side length")
    {
        const RectangleMatrix coefficients(2, 2);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{1.0}),
            std::invalid_argument
        );
    }

    SECTION("Rejects non-finite diagonal coefficients")
    {
        constexpr double data[1][1] = {{std::numeric_limits<double>::infinity()}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{1.0}),
            std::invalid_argument
        );
    }

    SECTION("Rejects non-finite off-diagonal coefficients")
    {
        constexpr double data[2][2] = {
            {1.0, std::numeric_limits<double>::infinity()},
            {0.0, 1.0}
        };
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{1.0, 1.0}),
            std::invalid_argument
        );
    }

    SECTION("Rejects non-finite right-hand-side values")
    {
        constexpr double data[1][1] = {{1.0}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{std::numeric_limits<double>::infinity()}),
            std::invalid_argument
        );
    }

    SECTION("Returns an empty solution for an empty system")
    {
        const RectangleMatrix coefficients(0, 0);

        REQUIRE(resolver.resolve(coefficients, Vector(0)).size() == 0);
    }
}

TEST_CASE("Seidel resolver reports non-convergent iterations")
{
    constexpr double data[2][2] = {{1.0, 2.0}, {2.0, 1.0}};
    const RectangleMatrix coefficients(data);

    REQUIRE_THROWS_AS(
        resolver.resolve(coefficients, Vector{1.0, 0.0}),
        std::runtime_error
    );
}
