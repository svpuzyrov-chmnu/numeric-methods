#pragma once

#include <functional>
#include <limits>
#include <vector>
#include "matrix.hpp"

namespace nonlinear::core
{
    using linear_algebra::matrix::simple::Matrix;
    using linear_algebra::matrix::simple::Vector;
    using num_limis = std::numeric_limits<double>;

    using NonlinearFunction = std::function<Vector(const Vector&)>;

    using LinearResolver = std::function<Vector(const Matrix&, const Vector&)>;

    struct NonlinearSystem
    {
        std::size_t dimension;
        NonlinearFunction function;
    };

    struct IterationOptions
    {
        std::size_t max_iterations = 100;
        double tolerance = 1e-8;
        double jacobian_step = 1e-6;
        double min_damping = 1.0 / 1024.0;
    };

    enum class ConvergenceStatus
    {
        CONVERGED,
        MAX_ITERS,
        SINGULAR_JACOBIAN,
        STAGNATION,
        NON_FINITE_VALUE
    };

    struct IterationResult
    {
        Vector solution;

        std::size_t iterations = 0;

        double residual_norm = num_limis::infinity();

        ConvergenceStatus status = ConvergenceStatus::MAX_ITERS;

        [[nodiscard]] bool converged() const
        {
            return status == ConvergenceStatus::CONVERGED;
        }
    };
}
