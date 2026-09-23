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

    constexpr double data[3][4] = {
        {1.0, 2.0, 3.0, 4.0},
        {5.0, 6.0, 7.0, 8.0},
        {9.0, 10.0, 11.0, 12.0}
    };

    SECTION("Construct with initializer list")
    {

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

    SECTION("Change rows")
    {
        RectangleMatrix m = data;

        REQUIRE(m.rows() == 3);

        REQUIRE(m.cols() == 4);

        const size_t& source_row = 0;
        const size_t& target_row = 2;

        m.change_rows(source_row, target_row);

        for (size_t i = 0; i < m.cols(); ++i)
        {
            REQUIRE(m(target_row, i) == data[source_row][i]);
            REQUIRE(m(source_row, i) == data[target_row][i]);
        }
    }

    SECTION("Change cols")
    {
        RectangleMatrix m = data;

        REQUIRE(m.rows() == 3);

        REQUIRE(m.cols() == 4);

        const size_t& source_col = 1;
        const size_t& target_col = 3;

        m.change_cols(source_col, target_col);

        for (size_t i = 0; i < m.rows(); ++i)
        {
            REQUIRE(m(i, target_col) == data[i][source_col]);
            REQUIRE(m(i, source_col) == data[i][target_col]);
        }
    }
}
