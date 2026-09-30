#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "gauss-all.hpp"
#include "matrix-all.hpp"
#include "gauss-exception.hpp"

#include <limits>
#include <stdexcept>

using linear_algebra::gauss::PartialGaussResolver;
using linear_algebra::matrix::RectangleMatrix;
using linear_algebra::vector::Vector;

namespace {
    const PartialGaussResolver resolver;

    RectangleMatrix make_pivoting_matrix(const size_t size) {
        return RectangleMatrix(
            size,
            size,
            [](const size_t &, const size_t &) { return true; },
            [size](const size_t &row, const size_t &col) {
                if (col == 0) {
                    return row == 0 ? 1.0 : (row == size - 1 ? 10.0 : 0.0);
                }
                return row == col ? 2.0 : 0.0;
            }
        );
    }

    Vector make_right_hand_side(const RectangleMatrix &coefficients, const Vector &expected) {
        return coefficients * expected;
    }

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

TEST_CASE("Partial Gauss resolver solves pivoting systems of the specific linear systems") {

    SECTION("Resolve system size 4") {

        RectangleMatrix coefficients = raw_m_4x4;

        Vector rhs{2.5603, -10.1311, 8.5549, 18.4019};

        auto solution = resolver.resolve(coefficients, rhs);

        auto back_rsh = make_right_hand_side(coefficients, solution);

        REQUIRE(solution.size() == back_rsh.size());

        for (size_t i = 0; i < solution.size(); ++i) {
            REQUIRE(rhs[i] == Catch::Approx(back_rsh[i]));
        }
    }

    SECTION("Resolve system size 5") {

        RectangleMatrix coefficients = raw_m5x5;

        Vector rhs{1.521, -0.0161, 2.4571, 13.7487, 1.0547};

        auto solution = resolver.resolve(coefficients, rhs);

        auto back_rsh = make_right_hand_side(coefficients, solution);

        REQUIRE(solution.size() == back_rsh.size());

        for (size_t i = 0; i < solution.size(); ++i) {
            REQUIRE(rhs[i] == Catch::Approx(back_rsh[i]));
        }
    }

}

TEST_CASE("Partial Gauss resolver solves pivoting systems of sizes 2 through 5") {
    const size_t size = GENERATE(size_t{2}, size_t{3}, size_t{4}, size_t{5});
    DYNAMIC_SECTION("Solves a " << size << "x" << size << " system") {
        auto coefficients = make_pivoting_matrix(size);
        Vector expected(size);
        for (size_t i = 0; i < size; ++i) {
            expected[i] = static_cast<double>(i + 1);
        }
        const auto rhs = make_right_hand_side(coefficients, expected);

        const auto solution = resolver.resolve(coefficients, rhs);

        REQUIRE(solution.size() == size);
        for (size_t i = 0; i < size; ++i) {
            REQUIRE(solution[i] == Catch::Approx(expected[i]));
        }

        REQUIRE(coefficients(0, 0) == 1.0);
        REQUIRE(coefficients(size - 1, 0) == 10.0);
    }
}

TEST_CASE("Partial Gauss resolver handles systems without a row swap") {
    const double data[3][3] = {
        {4.0, 1.0, -1.0},
        {2.0, 5.0, 1.0},
        {1.0, -2.0, 6.0}
    };

    const RectangleMatrix coefficients(data);

    const Vector expected{2.0, -1.0, 3.0};

    const auto right_hand_side = make_right_hand_side(coefficients, expected);

    const auto solution =resolver.resolve(coefficients, right_hand_side);

    for (size_t i = 0; i < expected.size(); ++i) {
        REQUIRE(solution[i] == Catch::Approx(expected[i]));
    }
}

TEST_CASE("Partial Gauss resolver classifies singular systems") {
    SECTION("Reports infinitely many solutions") {
        const double data[2][2] = {{1.0, 1.0}, {2.0, 2.0}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{2.0, 4.0}),
            linear_algebra::gauss::exception::xInfinitySolution
        );
    }

    SECTION("Reports no solution") {
        const double data[2][2] = {{1.0, 1.0}, {2.0, 2.0}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{2.0, 5.0}),
            linear_algebra::gauss::exception::xNoSolution
        );
    }
}

TEST_CASE("Partial Gauss resolver validates dimensions and finite diagonal inputs") {
    SECTION("Rejects non-square matrices") {
        const double data[2][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{1.0, 2.0}),
            std::invalid_argument
        );
    }

    SECTION("Rejects mismatched right-hand-side length") {
        const RectangleMatrix coefficients(2, 2);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{1.0}),
            std::invalid_argument
        );
    }

    SECTION("Rejects non-finite diagonal coefficients") {
        const double data[1][1] = {{std::numeric_limits<double>::infinity()}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{1.0}),
            std::invalid_argument
        );
    }

    SECTION("Rejects non-finite right-hand-side values") {
        const double data[1][1] = {{1.0}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(
            resolver.resolve(coefficients, Vector{std::numeric_limits<double>::infinity()}),
            std::invalid_argument
        );
    }

    SECTION("Returns an empty solution for an empty system") {
        const RectangleMatrix coefficients(0, 0);

        REQUIRE(resolver.resolve(coefficients, Vector(0)).size() == 0);
    }
}
