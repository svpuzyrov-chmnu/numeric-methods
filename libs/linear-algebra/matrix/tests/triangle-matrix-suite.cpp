#include <catch2/catch_test_macros.hpp>

#include "lower-triangle-matrix.hpp"
#include "upper-triangle-matrix.hpp"

using linear_algebra::matrix::LowerTriangleMatrix;
using linear_algebra::matrix::UpperTriangleMatrix;

TEST_CASE("Triangular matrix compact storage")
{
    SECTION("Upper triangle maps logical columns to compact row storage")
    {
        UpperTriangleMatrix matrix(3, 3);
        for (size_t row = 0; row < matrix.rows(); ++row)
        {
            matrix.init_row(row, [](size_t i, size_t j) { return i * 10.0 + j; });
        }

        const auto& read_only = matrix;
        REQUIRE(matrix(0, 2) == 2.0);
        REQUIRE(matrix(1, 2) == 12.0);
        REQUIRE(matrix(2, 2) == 22.0);
        REQUIRE(read_only(2, 0) == 0.0);
    }

    SECTION("Lower triangle initializes and maps its diagonal")
    {
        LowerTriangleMatrix matrix(3, 3);
        for (size_t row = 0; row < matrix.rows(); ++row)
        {
            matrix.init_row(row, [](size_t i, size_t j) { return i * 10.0 + j; });
        }

        const auto& read_only = matrix;
        REQUIRE(matrix(0, 0) == 0.0);
        REQUIRE(matrix(1, 1) == 11.0);
        REQUIRE(matrix(2, 2) == 22.0);
        REQUIRE(read_only(0, 1) == 0.0);
    }

    SECTION("Rectangular upper triangle has no rows past the diagonal")
    {
        UpperTriangleMatrix matrix(3, 2);
        for (size_t row = 0; row < matrix.rows(); ++row)
        {
            matrix.init_row(row, [](size_t i, size_t j) { return i * 10.0 + j; });
        }

        const auto& read_only = matrix;
        REQUIRE(read_only(2, 0) == 0.0);
        REQUIRE(read_only(2, 1) == 0.0);
    }
}
