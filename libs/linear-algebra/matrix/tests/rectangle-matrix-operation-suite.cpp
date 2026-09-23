#include <catch2/catch_test_macros.hpp>

#include "rectangle-matrix.hpp"
#include "catch2/generators/catch_generators.hpp"

TEST_CASE("Matrix operations")
{
    using linear_algebra::matrix::RectangleMatrix;

    constexpr double data1[3][4] = {
        {-10, 4.3, 3.0, -4.0},
        {1, 17, 13.9, 9.5},
        {8.5, -4, 22.11, 91.45}
    };

    constexpr double data2[3][4] = {
        {-5, 0.5, 20.08, 15.18},
        {7.12, 17, 13.9, -7.5},
        {14.22, -21.45, 11.20, 4.95}
    };

    SECTION("Sum of matrices")
    {
        RectangleMatrix m1 = data1;
        REQUIRE(m1.rows() == 3);
        REQUIRE(m1.cols() == 4);

        RectangleMatrix m2 = data2;
        REQUIRE(m2.rows() == 3);
        REQUIRE(m2.cols() == 4);

        RectangleMatrix result = m1 + m2;
        REQUIRE(result.rows() == 3);
        REQUIRE(result.cols() == 4);

        for (size_t i = 0; i < result.rows(); ++i)
        {
            for (size_t j = 0; j < result.cols(); ++j)
            {
                REQUIRE(result(i, j) == data1[i][j] + data2[i][j]);
            }
        }
    }

    SECTION("Difference of matrices")
    {
        RectangleMatrix m1 = data1;
        REQUIRE(m1.rows() == 3);
        REQUIRE(m1.cols() == 4);

        RectangleMatrix m2 = data2;
        REQUIRE(m2.rows() == 3);
        REQUIRE(m2.cols() == 4);

        RectangleMatrix result = m1 - m2;
        REQUIRE(result.rows() == 3);
        REQUIRE(result.cols() == 4);

        for (size_t i = 0; i < result.rows(); ++i)
        {
            for (size_t j = 0; j < result.cols(); ++j)
            {
                REQUIRE(result(i, j) == data1[i][j] - data2[i][j]);
            }
        }
    }

    SECTION("Success multiplication of matrices")
    {
        constexpr double data3[4][5] = {
            {-8.0, 4.3, 3.0, -4.0, 1.0},
            {1.0, 17.0, 13.9, 9.5, 2.0},
            {8.5, -4.0, 22.11, 91.45, 3.0},
            {5.0, 6.0, 7.0, 8.0, 4.0}
        };

        RectangleMatrix m1 = data1;
        REQUIRE(m1.rows() == 3);
        REQUIRE(m1.cols() == 4);

        RectangleMatrix m2 = data3;
        REQUIRE(m2.rows() == 4);
        REQUIRE(m2.cols() == 5);

        RectangleMatrix result = m1 * m2;
        REQUIRE(result.rows() == 3);
        REQUIRE(result.cols() == 5);

        for (size_t i = 0; i < result.rows(); ++i)
        {
            for (size_t j = 0; j < result.cols(); ++j)
            {
                double sum = 0;
                for (size_t k = 0; k < m1.cols(); ++k)
                {
                    sum += data1[i][k] * data3[k][j];
                }
                REQUIRE(result(i, j) == sum);
            }
        }
    }

    SECTION("Success multiplication matrix by scalar")
    {
        const auto multiplier = GENERATE(-5, 11, 2, 8, 15.4, -0.4, 2.7);

        RectangleMatrix m = data1;
        REQUIRE(m.rows() == 3);
        REQUIRE(m.cols() == 4);

        RectangleMatrix result = m * multiplier;
        REQUIRE(result.rows() == 3);
        REQUIRE(result.cols() == 4);

        for (size_t i = 0; i < result.rows(); ++i)
        {
            for (size_t j = 0; j < result.cols(); ++j)
            {
                REQUIRE(result(i, j) == data1[i][j] * multiplier);
            }
        }
    }
}
