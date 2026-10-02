#include <catch2/catch_test_macros.hpp>

#include "band-matrix.hpp"

#include <sstream>
#include <stdexcept>
#include <vector>

using linear_algebra::matrix::band::BandMatrix;

TEST_CASE("Band matrix stores values only inside its bandwidth")
{
    BandMatrix<double> matrix(4, 1, 2);

    matrix(0, 0) = 1.0;
    matrix(0, 1) = 2.0;
    matrix(0, 2) = 3.0;
    matrix(1, 0) = 4.0;
    matrix(1, 1) = 5.0;
    matrix(1, 2) = 6.0;
    matrix(1, 3) = 7.0;
    matrix(2, 1) = 8.0;
    matrix(2, 2) = 9.0;
    matrix(2, 3) = 10.0;
    matrix(3, 2) = 11.0;
    matrix(3, 3) = 12.0;

    REQUIRE(matrix(0, 0) == 1.0);
    REQUIRE(matrix(0, 2) == 3.0);
    REQUIRE(matrix(1, 0) == 4.0);
    REQUIRE(matrix(1, 3) == 7.0);
    REQUIRE(matrix(2, 1) == 8.0);
    REQUIRE(matrix(3, 3) == 12.0);

    const auto& read_only = matrix;
    REQUIRE(read_only(0, 3) == 0.0);
    REQUIRE(read_only(2, 0) == 0.0);
    REQUIRE(read_only(3, 1) == 0.0);
}

TEST_CASE("Band matrix rejects invalid mutable indices")
{
    BandMatrix<double> matrix(3, 1, 1);

    SECTION("Rejects writes outside the band without aliasing another element")
    {
        matrix(1, 1) = 5.0;
        REQUIRE_THROWS_AS(matrix(0, 2) = 9.0, std::out_of_range);
        REQUIRE(matrix(1, 1) == 5.0);
    }

    SECTION("Rejects coordinates outside the matrix")
    {
        REQUIRE_THROWS_AS(matrix(3, 0) = 1.0, std::out_of_range);
        REQUIRE_THROWS_AS(matrix(0, 3) = 1.0, std::out_of_range);
    }

    SECTION("Const reads outside the matrix throw")
    {
        const auto& read_only = matrix;
        REQUIRE_THROWS_AS(read_only(3, 0), std::out_of_range);
        REQUIRE_THROWS_AS(read_only(0, 3), std::out_of_range);
    }
}

TEST_CASE("Band matrix supports zero bandwidth and stream output")
{
    SECTION("Zero bandwidth stores only diagonal elements")
    {
        BandMatrix<int> diagonal(3, 0, 0);
        diagonal(0, 0) = 1;
        diagonal(1, 1) = 2;
        diagonal(2, 2) = 3;

        const auto& read_only = diagonal;
        REQUIRE(read_only(0, 1) == 0);
        REQUIRE(read_only(1, 0) == 0);
    }

    SECTION("Prints the full logical matrix including implicit zeros")
    {
        BandMatrix<int> matrix(2, 0, 1);
        matrix(0, 0) = 1;
        matrix(0, 1) = 2;
        matrix(1, 1) = 3;

        std::ostringstream output;
        output << matrix;

        REQUIRE(output.str() == "1 2\n0 3\n");
    }
}

TEST_CASE("Band matrix constructs from diagonal bands")
{
    const std::vector<std::vector<double>> bands{
        {0.0, 10.0, 20.0, 30.0},
        {1.0, 2.0, 3.0, 4.0},
        {40.0, 50.0, 60.0, 0.0}
    };
    const BandMatrix<double> matrix(4, bands);

    REQUIRE(matrix(0, 0) == 1.0);
    REQUIRE(matrix(1, 0) == 10.0);
    REQUIRE(matrix(1, 1) == 2.0);
    REQUIRE(matrix(0, 1) == 40.0);
    REQUIRE(matrix(2, 3) == 60.0);
    REQUIRE(matrix(3, 2) == 30.0);
    REQUIRE(matrix(3, 3) == 4.0);

    REQUIRE(matrix(0, 3) == 0.0);
    REQUIRE(matrix(3, 0) == 0.0);
}

TEST_CASE("Band matrix constructs a diagonal matrix from one band")
{
    const BandMatrix<int> matrix(3, std::vector<std::vector<int>>{{4, 5, 6}});

    REQUIRE(matrix(0, 0) == 4);
    REQUIRE(matrix(1, 1) == 5);
    REQUIRE(matrix(2, 2) == 6);
    REQUIRE(matrix(0, 1) == 0);
    REQUIRE(matrix(1, 0) == 0);
}
