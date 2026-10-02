#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "qr-decomposition.hpp"

#include <limits>
#include <stdexcept>

using linear_algebra::matrix::RectangleMatrix;
using linear_algebra::matrix::decomposition::qr::QRDecomposition;

namespace
{
    const QRDecomposition decomposition;

    void require_qr_factorization(const RectangleMatrix& matrix)
    {
        const auto result = decomposition.decompose(matrix);

        REQUIRE(result.q.rows() == matrix.rows());
        REQUIRE(result.q.cols() == matrix.rows());
        REQUIRE(result.r.rows() == matrix.rows());
        REQUIRE(result.r.cols() == matrix.cols());

        for (size_t row = 0; row < matrix.rows(); ++row)
        {
            for (size_t col = 0; col < matrix.cols(); ++col)
            {
                double product = 0.0;
                for (size_t inner = 0; inner < matrix.rows(); ++inner)
                {
                    product += result.q(row, inner) * result.r(inner, col);
                }

                REQUIRE(product == Catch::Approx(matrix(row, col)).margin(1e-10));
            }
        }

        for (size_t row = 0; row < matrix.rows(); ++row)
        {
            for (size_t col = 0; col < matrix.rows(); ++col)
            {
                double dot_product = 0.0;
                for (size_t inner = 0; inner < matrix.rows(); ++inner)
                {
                    dot_product += result.q(inner, row) * result.q(inner, col);
                }

                REQUIRE(dot_product == Catch::Approx(row == col ? 1.0 : 0.0).margin(1e-10));
            }
        }
    }
}

TEST_CASE("QR decomposition factors square and rectangular matrices")
{
    SECTION("Factors a square matrix")
    {
        constexpr double data[3][3] = {
            {12.0, -51.0, 4.0},
            {6.0, 167.0, -68.0},
            {-4.0, 24.0, -41.0}
        };
        const RectangleMatrix matrix(data);

        require_qr_factorization(matrix);
    }

    SECTION("Factors a tall matrix")
    {
        constexpr double data[4][3] = {
            {1.0, 2.0, -1.0},
            {2.0, 1.0, 0.0},
            {-1.0, 3.0, 2.0},
            {4.0, -2.0, 1.0}
        };
        const RectangleMatrix matrix(data);

        require_qr_factorization(matrix);
    }

    SECTION("Factors a wide matrix")
    {
        constexpr double data[2][3] = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0}
        };
        const RectangleMatrix matrix(data);

        require_qr_factorization(matrix);
    }

    SECTION("Handles a rank-deficient matrix")
    {
        constexpr double data[3][2] = {
            {1.0, 2.0},
            {2.0, 4.0},
            {3.0, 6.0}
        };
        const RectangleMatrix matrix(data);

        require_qr_factorization(matrix);
    }
}

TEST_CASE("QR decomposition handles empty and rejects non-finite matrices")
{
    SECTION("Handles an empty matrix")
    {
        const RectangleMatrix matrix(0, 0);

        const auto result = decomposition.decompose(matrix);

        REQUIRE(result.q.rows() == 0);
        REQUIRE(result.q.cols() == 0);
        REQUIRE(result.r.rows() == 0);
        REQUIRE(result.r.cols() == 0);
    }

    SECTION("Rejects non-finite entries")
    {
        constexpr double data[1][1] = {{std::numeric_limits<double>::infinity()}};
        const RectangleMatrix matrix(data);

        REQUIRE_THROWS_AS(decomposition.decompose(matrix), std::invalid_argument);
    }
}
