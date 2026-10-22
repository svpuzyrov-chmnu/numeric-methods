#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "matrix-reader.hpp"
#include "matrix-writer.hpp"

#include <sstream>
#include <stdexcept>

using linear_algebra::matrix::simple::ExtendMatrix;
using linear_algebra::matrix::simple::Matrix;
using linear_algebra::matrix::simple::Vector;
using linear_algebra::matrix::simple::make_identity;
using linear_algebra::matrix::simple::make_matrix;
using linear_algebra::matrix::simple::operator*;
using linear_algebra::matrix::simple::operator-;

TEST_CASE("Simple matrix helpers create generated and identity matrices")
{
    const Matrix generated = make_matrix(
        2,
        3,
        [](const size_t& row, const size_t& column)
        {
            return static_cast<double>(row * 3 + column);
        });

    REQUIRE((generated == Matrix{{0.0, 1.0, 2.0}, {3.0, 4.0, 5.0}}));
    REQUIRE((make_identity(3) == Matrix{
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}));
    REQUIRE(make_identity(0).empty());
}

TEST_CASE("Simple matrix multiplication computes rectangular products")
{
    const Matrix left{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    const Matrix right{{7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0}};

    REQUIRE((left * right == Matrix{{58.0, 64.0}, {139.0, 154.0}}));
}

TEST_CASE("Simple matrix multiplication rejects incompatible dimensions")
{
    const Matrix left{{1.0, 2.0}, {3.0, 4.0}};
    const Matrix right{{1.0, 2.0, 3.0}};

    REQUIRE_THROWS_AS(left * right, std::invalid_argument);
}

TEST_CASE("Simple matrix-vector multiplication computes the product")
{
    const Matrix matrix{{1.0, 2.0}, {3.0, 4.0}};
    const Vector vector{5.0, 6.0};

    const Vector result = matrix * vector;

    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == Catch::Approx(17.0));
    REQUIRE(result[1] == Catch::Approx(39.0));
}

TEST_CASE("Simple matrix-vector multiplication supports rectangular matrices")
{
    const Matrix matrix{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

    REQUIRE((matrix * Vector{1.0, 2.0, 3.0} == Vector{14.0, 32.0}));
}

TEST_CASE("Simple matrix-vector multiplication rejects incompatible dimensions")
{
    REQUIRE_THROWS_AS((Matrix{{1.0, 2.0}, {3.0, 4.0}} * Vector{3.0}), std::invalid_argument);
}

TEST_CASE("Simple vector subtraction subtracts matching elements")
{
    REQUIRE(((Vector{5.0, 3.0, 1.0} - Vector{2.0, 1.0, 4.0}) == Vector{3.0, 2.0, -3.0}));
}

TEST_CASE("Simple vector subtraction uses the shorter input length")
{
    REQUIRE(((Vector{5.0, 3.0} - Vector{2.0}) == Vector{3.0}));
    REQUIRE((Vector{} - Vector{2.0}).empty());
}

TEST_CASE("Simple matrix readers parse vectors, matrices, and augmented matrices")
{
    SECTION("Vector")
    {
        std::istringstream input("1.5 -2 3\nignored");
        REQUIRE((
            linear_algebra::matrix::simple::reader::read_vector(input) ==
            Vector{1.5, -2.0, 3.0}));
    }

    SECTION("Matrix")
    {
        std::istringstream input("1 2\n\n3 4\n");
        REQUIRE((
            linear_algebra::matrix::simple::reader::read_matrix(input) ==
            Matrix{{1.0, 2.0}, {3.0, 4.0}}));
    }

    SECTION("Augmented matrix")
    {
        std::istringstream input("1 2 5\n3 4 6\n");
        const ExtendMatrix result =
            linear_algebra::matrix::simple::reader::read_ext_matrix(input);

        REQUIRE((result.matrix == Matrix{{1.0, 2.0}, {3.0, 4.0}}));
        REQUIRE((result.vector == Vector{5.0, 6.0}));
    }
}

TEST_CASE("Simple matrix readers reject malformed or non-finite data")
{
    SECTION("Inconsistent matrix row widths")
    {
        std::istringstream input("1 2\n3\n");
        REQUIRE_THROWS_AS(
            linear_algebra::matrix::simple::reader::read_matrix(input),
            std::runtime_error);
    }

    SECTION("Non-numeric matrix data")
    {
        std::istringstream input("1 invalid\n");
        REQUIRE_THROWS_AS(
            linear_algebra::matrix::simple::reader::read_matrix(input),
            std::runtime_error);
    }

    SECTION("Augmented rows require coefficients and a right-hand side")
    {
        std::istringstream input("1\n");
        REQUIRE_THROWS_AS(
            linear_algebra::matrix::simple::reader::read_ext_matrix(input),
            std::runtime_error);
    }
}

TEST_CASE("Simple matrix writers serialize vectors and matrices")
{
    SECTION("Vector")
    {
        std::ostringstream output;
        linear_algebra::matrix::simple::writer::write_to(output, Vector{1.0, 2.0});
        REQUIRE(output.str() == "              1              2");
    }

    SECTION("Matrix")
    {
        std::ostringstream output;
        linear_algebra::matrix::simple::writer::write_to(
            output, Matrix{{1.0, 2.0}, {3.0, 4.0}});
        std::istringstream input(output.str());
        REQUIRE((
            linear_algebra::matrix::simple::reader::read_matrix(
                input) ==
            Matrix{{1.0, 2.0}, {3.0, 4.0}}));
    }
}
