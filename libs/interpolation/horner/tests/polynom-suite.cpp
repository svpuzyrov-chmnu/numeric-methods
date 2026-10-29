#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "polynom.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

TEST_CASE("Polynomial evaluates coefficients in ascending degree order")
{
    const polynomial::Polynomial polynomial({-1.0, 3.0, 2.0});

    REQUIRE(polynomial.evaluate(2.0) == Catch::Approx(13.0));
    REQUIRE(polynomial(2.0) == Catch::Approx(13.0));
}

TEST_CASE("Polynomial evaluates constant, zero, and sparse polynomials")
{
    SECTION("constant polynomial")
    {
        const polynomial::Polynomial polynomial({-3.5});

        REQUIRE(polynomial.evaluate(-2.0) == Catch::Approx(-3.5));
    }

    SECTION("zero polynomial")
    {
        const polynomial::Polynomial polynomial({0.0, 0.0, 0.0});

        REQUIRE(polynomial.evaluate(-2.0) == Catch::Approx(0.0));
    }

    SECTION("evaluation at zero returns the constant coefficient")
    {
        const polynomial::Polynomial polynomial({4.25, -2.0, 3.0});

        REQUIRE(polynomial.evaluate(0.0) == Catch::Approx(4.25));
    }

    SECTION("sparse polynomial evaluated at a negative value")
    {
        const polynomial::Polynomial polynomial({1.0, 0.0, -2.0, 0.0, 1.0});

        REQUIRE(polynomial(-2.0) == Catch::Approx(9.0));
    }
}

TEST_CASE("Polynomial reports its degree")
{
    SECTION("constant polynomial")
    {
        const polynomial::Polynomial polynomial({5.0});

        REQUIRE(polynomial.degree() == 0);
    }

    SECTION("polynomial with several terms")
    {
        const polynomial::Polynomial polynomial({1.0, -2.0, 0.0, 4.0});

        REQUIRE(polynomial.degree() == 3);
    }
}

TEST_CASE("Polynomial exposes its coefficients")
{
    const polynomial::Coefficients coefficients{1.0, -2.0, 0.0, 4.0};
    const polynomial::Polynomial polynomial(coefficients);

    REQUIRE(polynomial.coefficients() == coefficients);
}

TEST_CASE("Polynomial owns its coefficients")
{
    polynomial::Coefficients source{1.0, 2.0};
    const polynomial::Polynomial polynomial(source);
    source[0] = 10.0;

    auto returnedCoefficients = polynomial.coefficients();
    returnedCoefficients[1] = 20.0;

    REQUIRE((polynomial.coefficients() == polynomial::Coefficients{1.0, 2.0}));
    REQUIRE(polynomial(2.0) == Catch::Approx(5.0));
}

TEST_CASE("Polynomial rejects an empty coefficient list")
{
    REQUIRE_THROWS_AS(polynomial::Polynomial({}), std::invalid_argument);
}

TEST_CASE("Polynomial rejects non-finite coefficients during evaluation")
{
    const polynomial::Polynomial polynomial({std::numeric_limits<double>::infinity(), 1.0});

    REQUIRE_THROWS_AS(polynomial.evaluate(1.0), std::invalid_argument);
}
