#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "polynom.hpp"

#include <stdexcept>

TEST_CASE("Polynomial evaluates coefficients in ascending degree order")
{
    const polynomial::Polynomial polynomial({-1.0, 3.0, 2.0});

    REQUIRE(polynomial.evaluate(2.0) == Catch::Approx(13.0));
    REQUIRE(polynomial(2.0) == Catch::Approx(13.0));
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

TEST_CASE("Polynomial rejects an empty coefficient list")
{
    REQUIRE_THROWS_AS(polynomial::Polynomial({}), std::invalid_argument);
}
