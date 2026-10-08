#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "lu-decomposition.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

using linear_algebra::matrix::RectangleMatrix;
using linear_algebra::matrix::decomposition::lu::LUDecomposition;

namespace
{
    const LUDecomposition decomposition;

    std::vector<size_t> inverse_pivoting(const std::vector<size_t>& pivot_indices)
    {
        std::vector<size_t> inverse(pivot_indices.size(), 0);
        for (size_t row = 0; row < pivot_indices.size(); ++row)
        {
            inverse[pivot_indices[row]] = row;
        }
        return inverse;
    }

    void require_factorization(const RectangleMatrix& coefficients)
    {
        const auto [lower, upper, pivot_indices] = decomposition.decompose(coefficients);
        const auto size = coefficients.rows();
        const auto inverse_pivot_indices = inverse_pivoting(pivot_indices);

        REQUIRE(pivot_indices.size() == size);

        for (size_t original_row = 0; original_row < size; ++original_row)
        {
            const auto pivoted_row = inverse_pivot_indices[original_row];

            for (size_t col = 0; col < size; ++col)
            {
                double product = 0.0;
                for (size_t inner = 0; inner < size; ++inner)
                {
                    product += lower(pivoted_row, inner) * upper(inner, col);
                }

                REQUIRE(product == Catch::Approx(coefficients(original_row, col)).margin(1e-10));
            }
        }
    }
}

TEST_CASE("LU decomposition factors square matrices")
{
    SECTION("Factors a matrix without pivoting")
    {
        constexpr double data[3][3] = {
            {4.0, 3.0, 2.0},
            {2.0, 5.0, 1.0},
            {1.0, 2.0, 6.0}
        };

        const RectangleMatrix coefficients = data;

        const auto [lower, upper, pivot_indices] = decomposition.decompose(coefficients);

        const std::vector<size_t> expected_pivots{0, 1, 2};

        REQUIRE(pivot_indices == expected_pivots);

        for (size_t i = 0; i < coefficients.rows(); ++i)
        {
            REQUIRE(lower(i, i) == Catch::Approx(1.0));
        }
        require_factorization(coefficients);
    }

    SECTION("Applies partial pivoting")
    {
        constexpr double data[3][3] = {
            {0.0, 2.0, 1.0},
            {1.0, 1.0, 0.0},
            {2.0, 0.0, 1.0}
        };
        const RectangleMatrix coefficients(data);

        const auto [lower, upper, pivot_indices] = decomposition.decompose(coefficients);

        const std::vector<size_t> expected_pivots{2, 0, 1};
        REQUIRE(pivot_indices == expected_pivots);
        require_factorization(coefficients);
    }

    SECTION("Factors larger matrices")
    {
        constexpr double data_4x4[4][4] = {
            {10.0, -1.0, 2.0, 0.0},
            {-1.0, 11.0, -1.0, 3.0},
            {2.0, -1.0, 10.0, -1.0},
            {0.0, 3.0, -1.0, 8.0}
        };
        constexpr double data_5x5[5][5] = {
            {12.0, 1.0, -1.0, 0.0, 2.0},
            {1.0, 13.0, 2.0, -1.0, 0.0},
            {-1.0, 2.0, 14.0, 1.0, -2.0},
            {0.0, -1.0, 1.0, 15.0, 1.0},
            {2.0, 0.0, -2.0, 1.0, 16.0}
        };

        const RectangleMatrix coefficients_4x4(data_4x4);
        const RectangleMatrix coefficients_5x5(data_5x5);
        require_factorization(coefficients_4x4);
        require_factorization(coefficients_5x5);
    }

    SECTION("Handles an empty matrix")
    {
        const RectangleMatrix coefficients(0, 0);

        const auto [lower, upper, pivot_indices] = decomposition.decompose(coefficients);

        REQUIRE(lower.rows() == 0);
        REQUIRE(upper.rows() == 0);
        REQUIRE(pivot_indices.empty());
    }
}

TEST_CASE("LU decomposition rejects invalid and singular matrices")
{
    SECTION("Rejects a non-square matrix")
    {
        constexpr double data[2][3] = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0}
        };
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(decomposition.decompose(coefficients), std::invalid_argument);
    }

    SECTION("Rejects a singular matrix")
    {
        constexpr double data[2][2] = {
            {1.0, 2.0},
            {2.0, 4.0}
        };
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(decomposition.decompose(coefficients), std::runtime_error);
    }

    SECTION("Rejects non-finite entries")
    {
        constexpr double data[1][1] = {{std::numeric_limits<double>::infinity()}};
        const RectangleMatrix coefficients(data);

        REQUIRE_THROWS_AS(decomposition.decompose(coefficients), std::invalid_argument);
    }
}
