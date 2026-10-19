#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "equation-bisect.hpp"

#include <cmath>
#include <functional>
#include <string>
#include <vector>

using equation::core::Range;
using equation::core::ResultType;
using equation::bisect::BisectResolver;

namespace
{
    struct RootCase
    {
        std::string name;
        std::function<double(const double&)> equation;
        double left;
        double right;
        double expected_root;
    };
}

TEST_CASE("Bisection resolver converges for logarithmic, trigonometric, exponential, and polynomial equations")
{
    const std::vector<RootCase> cases{
        {"log(x) - 1", [](const double& x) { return std::log(x) - 1.0; }, 2.0, 3.0, std::exp(1.0)},
        {"sin(x)", [](const double& x) { return std::sin(x); }, -0.7, 1.3, 0.0},
        {"cos(x)", [](const double& x) { return std::cos(x); }, 1.0, 2.0, std::acos(-1.0) / 2.0},
        {"exp(x) - 2", [](const double& x) { return std::exp(x) - 2.0; }, 0.0, 1.0, std::log(2.0)},
        {"x^2 - 2", [](const double& x) { return x * x - 2.0; }, 1.0, 2.0, std::sqrt(2.0)},
        {"x^3 - 2", [](const double& x) { return x * x * x - 2.0; }, 1.0, 2.0, std::cbrt(2.0)},
        {"(x - 1)(x + 2)", [](const double& x) { return (x - 1.0) * (x + 2.0); }, 0.5, 2.0, 1.0},
    };

    for (const auto& test_case : cases)
    {
        DYNAMIC_SECTION("Converges for " << test_case.name)
        {
            const BisectResolver resolver{1e-10, 100000};
            const auto result = resolver.resolve(test_case.equation, Range{test_case.left, test_case.right});

            REQUIRE(result.resultType() == ResultType::DEFINED);
            REQUIRE(result.value() == Catch::Approx(test_case.expected_root).margin(1e-8));
            REQUIRE(std::fabs(test_case.equation(result.value())) < 1e-8);
            REQUIRE(resolver.count_of_iterations() > 0);
            REQUIRE(resolver.count_of_iterations() <= 100000);
        }
    }
}

TEST_CASE("Bisection resolver returns roots at interval boundaries and midpoint")
{
    SECTION("Returns a root at the left boundary without iterating")
    {
        const BisectResolver resolver{1e-10};
        const auto result = resolver.resolve(
            [](const double& x) { return x - 1.0; }, Range{1.0, 2.0});

        REQUIRE(result.resultType() == ResultType::DEFINED);
        REQUIRE(result.value() == 1.0);
        REQUIRE(resolver.count_of_iterations() == 0);
    }

    SECTION("Returns a root at the right boundary without iterating")
    {
        const BisectResolver resolver{1e-10};
        const auto result = resolver.resolve(
            [](const double& x) { return x - 2.0; }, Range{1.0, 2.0});

        REQUIRE(result.resultType() == ResultType::DEFINED);
        REQUIRE(result.value() == 2.0);
        REQUIRE(resolver.count_of_iterations() == 0);
    }

    SECTION("Returns an exact root at the interval midpoint")
    {
        const BisectResolver resolver{1e-10};
        const auto result = resolver.resolve(
            [](const double& x) { return x; }, Range{-1.0, 1.0});

        REQUIRE(result.resultType() == ResultType::DEFINED);
        REQUIRE(result.value() == 0.0);
        REQUIRE(resolver.count_of_iterations() == 1);
    }
}

TEST_CASE("Bisection resolver returns an undefined result when endpoint values have the same sign")
{
    const BisectResolver resolver{1e-10};

    SECTION("Rejects an interval without a root")
    {
        const auto result = resolver.resolve(
            [](const double& x) { return x * x + 1.0; }, Range{-1.0, 1.0});

        REQUIRE(result.resultType() == ResultType::UNDEFINED);
        REQUIRE(resolver.count_of_iterations() == 0);
    }

    SECTION("Rejects an interior root that does not change sign across the interval")
    {
        const auto result = resolver.resolve(
            [](const double& x) { return x * x; }, Range{-1.0, 1.0});

        REQUIRE(result.resultType() == ResultType::UNDEFINED);
        REQUIRE(resolver.count_of_iterations() == 0);
    }
}

TEST_CASE("Bisection resolver resets its iteration count for each resolve call")
{
    const BisectResolver resolver{1e-10};

    const auto converged = resolver.resolve(
        [](const double& x) { return x - 0.25; }, Range{-1.0, 1.0});
    REQUIRE(converged.resultType() == ResultType::DEFINED);
    REQUIRE(resolver.count_of_iterations() > 0);

    const auto not_bracketed = resolver.resolve(
        [](const double& x) { return x * x + 1.0; }, Range{-1.0, 1.0});
    REQUIRE(not_bracketed.resultType() == ResultType::UNDEFINED);
    REQUIRE(resolver.count_of_iterations() == 0);
}
