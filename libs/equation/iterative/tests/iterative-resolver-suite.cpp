#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "equation-chord.hpp"

#include <cmath>
#include <functional>
#include <string>
#include <vector>

using equation::core::Range;
using equation::core::ResultType;
using equation::iterative::IterativeResolver;

namespace
{
    struct ConvergenceCase
    {
        std::string name;
        std::function<double(const double&)> equation;
        double left;
        double right;
        double factor;
        double expected_root;
    };
}

TEST_CASE("Iterative resolver converges for logarithmic, trigonometric, exponential, and polynomial equations")
{
    const std::vector<ConvergenceCase> cases{
        {"log(x) - 1, factor -0.5", [](const double& x) { return std::log(x) - 1.0; }, 2.0, 3.0, -0.5,
         std::exp(1.0)},
        {"log(x), factor -0.5", [](const double& x) { return std::log(x); }, 0.5, 1.8, -0.5, 1.0},
        {"sin(x), factor -1", [](const double& x) { return std::sin(x); }, -0.7, 1.3, -1.0, 0.0},
        {"cos(x), factor 1", [](const double& x) { return std::cos(x); }, 1.0, 2.0, 1.0, std::acos(-1.0) / 2.0},
        {"exp(x) - 2, factor -0.5", [](const double& x) { return std::exp(x) - 2.0; }, 0.0, 1.0, -0.5,
         std::log(2.0)},
        {"x^2 - 2, factor -0.25", [](const double& x) { return x * x - 2.0; }, 1.0, 2.0, -0.25,
         std::sqrt(2.0)},
        {"x^3 - 2, factor -0.1", [](const double& x) { return x * x * x - 2.0; }, 1.0, 2.0, -0.1,
         std::cbrt(2.0)},
        {"(x - 1)(x + 2), factor -0.2", [](const double& x) { return (x - 1.0) * (x + 2.0); }, 0.5, 2.0,
         -0.2, 1.0},
    };

    for (const auto& test_case : cases)
    {
        DYNAMIC_SECTION("Converges for " << test_case.name)
        {
            const IterativeResolver resolver{test_case.factor, 1e-10, 100000};
            const auto result = resolver.resolve(test_case.equation, Range{test_case.left, test_case.right});

            REQUIRE(result.resultType() == ResultType::DEFINED);
            REQUIRE(result.value() == Catch::Approx(test_case.expected_root).margin(1e-8));
            REQUIRE(std::fabs(test_case.equation(result.value())) < 1e-8);
            REQUIRE(resolver.count_of_iterations() > 0);
            REQUIRE(resolver.count_of_iterations() < 100000);
        }
    }
}

TEST_CASE("Iterative resolver returns roots at interval boundaries")
{
    SECTION("Returns a root at the left boundary without iterating")
    {
        const IterativeResolver resolver{-0.5};
        const auto result = resolver.resolve(
            [](const double& x) { return x - 1.0; }, Range{1.0, 2.0});

        REQUIRE(result.resultType() == ResultType::DEFINED);
        REQUIRE(result.value() == 1.0);
        REQUIRE(resolver.count_of_iterations() == 0);
    }

    SECTION("Returns a root at the right boundary without iterating")
    {
        const IterativeResolver resolver{-0.5};
        const auto result = resolver.resolve(
            [](const double& x) { return x - 2.0; }, Range{1.0, 2.0});

        REQUIRE(result.resultType() == ResultType::DEFINED);
        REQUIRE(result.value() == 2.0);
        REQUIRE(resolver.count_of_iterations() == 0);
    }
}

TEST_CASE("Iterative resolver returns an undefined result when endpoint values have the same sign")
{
    const IterativeResolver resolver{-0.5};

    SECTION("Rejects an interval without a root")
    {
        const auto result = resolver.resolve(
            [](const double& x) { return x * x + 1.0; }, Range{-1.0, 1.0});

        REQUIRE(result.resultType() == ResultType::UNDEFINED);
        REQUIRE(resolver.count_of_iterations() == 0);
    }

    SECTION("Rejects a root that does not change sign across the interval")
    {
        const auto result = resolver.resolve(
            [](const double& x) { return x * x; }, Range{-1.0, 1.0});

        REQUIRE(result.resultType() == ResultType::UNDEFINED);
        REQUIRE(resolver.count_of_iterations() == 0);
    }
}

TEST_CASE("Iterative resolver resets its iteration count for each resolve call")
{
    const IterativeResolver resolver{-0.5};

    const auto converged = resolver.resolve(
        [](const double& x) { return x - 0.25; }, Range{-1.0, 1.0});
    REQUIRE(converged.resultType() == ResultType::DEFINED);
    REQUIRE(resolver.count_of_iterations() > 0);

    const auto not_bracketed = resolver.resolve(
        [](const double& x) { return x * x + 1.0; }, Range{-1.0, 1.0});
    REQUIRE(not_bracketed.resultType() == ResultType::UNDEFINED);
    REQUIRE(resolver.count_of_iterations() == 0);
}
