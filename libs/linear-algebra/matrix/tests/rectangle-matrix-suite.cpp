#include <catch2/catch_test_macros.hpp>

#include "rectangle-matrix.hpp"

TEST_CASE("Matrix construction")
{
    using linear_algebra::matrix::RectangleMatrix;

    SECTION("Size constructor")
    {
        RectangleMatrix m(3, 4);
        REQUIRE(m.rows() == 3);
        REQUIRE(m.cols() == 4);
    }

    SECTION("Construct with generated items")
    {
        RectangleMatrix m(3, 4, [](const auto&, const auto&) { return true; }, [](const auto& row, const auto& col)
        {
            return 0.0 + row + col;
        });
        REQUIRE(m.rows() == 3);
        REQUIRE(m.cols() == 4);
        for (size_t i = 0; i < m.rows(); ++i)
        {
            for (size_t j = 0; j < m.cols(); ++j)
            {
                REQUIRE(m(i, j) == i + j);
            }
        }
    }

    SECTION("Construct with initializer list")
    {
        constexpr double data[3][4] = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
        };

        RectangleMatrix m = data;

        REQUIRE(m.rows() == 3);

        REQUIRE(m.cols() == 4);

        for (size_t i = 0; i < m.rows(); ++i)
        {
            for (size_t j = 0; j < m.cols(); ++j)
            {
                REQUIRE(m(i, j) == data[i][j]);
            }
        }
    }
}
