#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "gauss.hpp"
#include "non-linear-newton.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace
{
    using nonlinear::core::ConvergenceStatus;
    using nonlinear::newton::IterationOptions;
    using nonlinear::newton::IterationResult;
    using nonlinear::newton::LinearResolver;
    using nonlinear::newton::Matrix;
    using nonlinear::newton::NonlinearSystem;
    using nonlinear::newton::Vector;

    LinearResolver gauss_resolver()
    {
        return [](const Matrix& jacobian, const Vector& rhs)
        {
            return linear_algebra::gauss::simple::resolve(jacobian, rhs);
        };
    }
}

TEST_CASE("Newton resolver converges to a scalar nonlinear root")
{
    const NonlinearSystem system{
        1,
        [](const Vector& x)
        {
            return Vector{x[0] * x[0] - 2.0};
        }};

    const IterationResult result =
        nonlinear::newton::resolve(system, Vector{1.0}, gauss_resolver());

    REQUIRE(result.status == ConvergenceStatus::CONVERGED);
    REQUIRE(result.converged());
    REQUIRE(result.solution[0] == Catch::Approx(std::sqrt(2.0)).epsilon(1e-7));
    REQUIRE(result.residual_norm <= 1e-8);
    REQUIRE(result.iterations > 0);
}

TEST_CASE("Newton resolver solves a separable nonlinear system")
{
    const NonlinearSystem system{
        2,
        [](const Vector& x)
        {
            return Vector{x[0] * x[0] - 4.0, x[1] * x[1] - 9.0};
        }};

    const IterationResult result =
        nonlinear::newton::resolve(system, Vector{1.0, 1.0}, gauss_resolver());

    REQUIRE(result.status == ConvergenceStatus::CONVERGED);
    REQUIRE(result.solution[0] == Catch::Approx(2.0).epsilon(1e-7));
    REQUIRE(result.solution[1] == Catch::Approx(3.0).epsilon(1e-7));
}

TEST_CASE("Newton resolver solves a three-variable system with logarithm, cosine, and sine")
{
    const Vector expected{2.0, 0.5, 0.25};
    const NonlinearSystem system{
        3,
        [expected](const Vector& x)
        {
            return Vector{
                std::log(x[0]) - std::log(expected[0]),
                std::cos(x[1]) - std::cos(expected[1]),
                std::sin(x[2]) - std::sin(expected[2])};
        }};

    const IterationResult result =
        nonlinear::newton::resolve(system, Vector{1.5, 0.6, 0.3}, gauss_resolver());

    REQUIRE(result.status == ConvergenceStatus::CONVERGED);
    REQUIRE(result.solution[0] == Catch::Approx(expected[0]).epsilon(1e-7));
    REQUIRE(result.solution[1] == Catch::Approx(expected[1]).epsilon(1e-7));
    REQUIRE(result.solution[2] == Catch::Approx(expected[2]).epsilon(1e-7));
}

TEST_CASE("Newton resolver solves a four-variable system with logarithm, cosine, sine, and exponential")
{
    const Vector expected{2.0, 0.5, 0.25, 0.75};
    const NonlinearSystem system{
        4,
        [expected](const Vector& x)
        {
            return Vector{
                std::log(x[0]) - std::log(expected[0]),
                std::cos(x[1]) - std::cos(expected[1]),
                std::sin(x[2]) - std::sin(expected[2]),
                std::exp(x[3]) - std::exp(expected[3])};
        }};

    const IterationResult result =
        nonlinear::newton::resolve(system, Vector{1.5, 0.6, 0.3, 0.7}, gauss_resolver());

    REQUIRE(result.status == ConvergenceStatus::CONVERGED);
    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        REQUIRE(result.solution[i] == Catch::Approx(expected[i]).epsilon(1e-7));
    }
}

TEST_CASE("Newton resolver damps steps that do not reduce the residual")
{
    const NonlinearSystem system{
        1,
        [](const Vector& x)
        {
            return Vector{x[0] * x[0] * x[0] - 1.0};
        }};

    const IterationResult result =
        nonlinear::newton::resolve(system, Vector{0.1}, gauss_resolver());

    REQUIRE(result.status == ConvergenceStatus::CONVERGED);
    REQUIRE(result.solution[0] == Catch::Approx(1.0).epsilon(1e-7));
}

TEST_CASE("Newton resolver returns immediately for an initial solution")
{
    const NonlinearSystem system{
        1,
        [](const Vector& x)
        {
            return Vector{x[0] - 3.0};
        }};
    bool resolver_called = false;
    const LinearResolver resolver = [&resolver_called](const Matrix&, const Vector&)
    {
        resolver_called = true;
        return Vector{0.0};
    };

    const IterationResult result =
        nonlinear::newton::resolve(system, Vector{3.0}, resolver);

    REQUIRE(result.status == ConvergenceStatus::CONVERGED);
    REQUIRE(result.iterations == 0);
    REQUIRE(result.residual_norm == Catch::Approx(0.0));
    REQUIRE_FALSE(resolver_called);
}

TEST_CASE("Newton resolver respects the iteration limit")
{
    const NonlinearSystem system{
        1,
        [](const Vector& x)
        {
            return Vector{x[0] * x[0] - 2.0};
        }};
    IterationOptions options;
    options.max_iterations = 1;

    const IterationResult result = nonlinear::newton::resolve(
        system, Vector{1.0}, gauss_resolver(), options);

    REQUIRE(result.status == ConvergenceStatus::MAX_ITERS);
    REQUIRE_FALSE(result.converged());
    REQUIRE(result.iterations == 1);
    REQUIRE(result.residual_norm > options.tolerance);
}

TEST_CASE("Newton resolver reports stagnation when no step decreases the residual")
{
    const NonlinearSystem system{
        1,
        [](const Vector& x)
        {
            return Vector{x[0] * x[0] - 2.0};
        }};
    const LinearResolver zero_step = [](const Matrix&, const Vector&)
    {
        return Vector{0.0};
    };

    const IterationResult result =
        nonlinear::newton::resolve(system, Vector{1.0}, zero_step);

    REQUIRE(result.status == ConvergenceStatus::STAGNATION);
    REQUIRE(result.iterations == 0);
    REQUIRE(result.solution[0] == Catch::Approx(1.0));
}

TEST_CASE("Newton resolver reports a singular Jacobian when the linear resolver fails")
{
    const NonlinearSystem system{
        1,
        [](const Vector& x)
        {
            return Vector{x[0] * x[0] - 2.0};
        }};
    const LinearResolver failing_resolver = [](const Matrix&, const Vector&) -> Vector
    {
        throw std::runtime_error("singular system");
    };

    const IterationResult result =
        nonlinear::newton::resolve(system, Vector{1.0}, failing_resolver);

    REQUIRE(result.status == ConvergenceStatus::SINGULAR_JACOBIAN);
    REQUIRE(result.iterations == 0);
    REQUIRE(result.solution[0] == Catch::Approx(1.0));
}

TEST_CASE("Newton resolver rejects invalid input")
{
    const NonlinearSystem system{
        1,
        [](const Vector& x)
        {
            return Vector{x[0] - 1.0};
        }};

    SECTION("Initial vector has the wrong dimension")
    {
        REQUIRE_THROWS_AS(
            nonlinear::newton::resolve(system, Vector{1.0, 2.0}, gauss_resolver()),
            std::invalid_argument);
    }

    SECTION("Initial vector contains a non-finite value")
    {
        REQUIRE_THROWS_AS(
            nonlinear::newton::resolve(
                system, Vector{std::numeric_limits<double>::infinity()}, gauss_resolver()),
            std::invalid_argument);
    }

    SECTION("Linear resolver is not defined")
    {
        REQUIRE_THROWS_AS(
            nonlinear::newton::resolve(system, Vector{0.0}, {}),
            std::invalid_argument);
    }

    SECTION("System function is not defined")
    {
        REQUIRE_THROWS_AS(
            nonlinear::newton::resolve(
                NonlinearSystem{1, {}}, Vector{0.0}, gauss_resolver()),
            std::invalid_argument);
    }
}

TEST_CASE("Newton resolver reports non-finite values")
{
    SECTION("Initial function evaluation is non-finite")
    {
        const NonlinearSystem system{
            1,
            [](const Vector&)
            {
                return Vector{std::numeric_limits<double>::quiet_NaN()};
            }};

        const IterationResult result =
            nonlinear::newton::resolve(system, Vector{0.0}, gauss_resolver());

        REQUIRE(result.status == ConvergenceStatus::NON_FINITE_VALUE);
        REQUIRE(result.iterations == 0);
    }

    SECTION("Linear resolver returns a non-finite step")
    {
        const NonlinearSystem system{
            1,
            [](const Vector& x)
            {
                return Vector{x[0] - 1.0};
            }};
        const LinearResolver invalid_resolver = [](const Matrix&, const Vector&)
        {
            return Vector{std::numeric_limits<double>::quiet_NaN()};
        };

        const IterationResult result =
            nonlinear::newton::resolve(system, Vector{0.0}, invalid_resolver);

        REQUIRE(result.status == ConvergenceStatus::NON_FINITE_VALUE);
        REQUIRE(result.iterations == 0);
    }

    SECTION("Linear resolver returns a step with the wrong dimension")
    {
        const NonlinearSystem system{
            1,
            [](const Vector& x)
            {
                return Vector{x[0] - 1.0};
            }};
        const LinearResolver invalid_resolver = [](const Matrix&, const Vector&)
        {
            return Vector{0.0, 0.0};
        };

        const IterationResult result =
            nonlinear::newton::resolve(system, Vector{0.0}, invalid_resolver);

        REQUIRE(result.status == ConvergenceStatus::NON_FINITE_VALUE);
    }
}
