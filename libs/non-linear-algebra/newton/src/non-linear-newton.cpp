#include "non-linear-newton.hpp"
#include <stdexcept>
#include <vector>

#include "non-linear-utils.hpp"

namespace nonlinear::newton
{
    IterationResult resolve(const NonlinearSystem& system, const Vector& init,
                                 const LinearResolver& ls_resolver, const IterationOptions& options)
    {
            using core::utils::validate;
            using core::utils::is_finite;
            using core::utils::evaluate;
            using core::utils::norm;
            using core::utils::jacobian;
            using core::ConvergenceStatus;

            validate(system, options);

            if (init.size() != system.dimension)
            {
                throw std::invalid_argument(
                    "Incorrect initial vector dimension"
                );
            }

            if (!is_finite(init))
            {
                throw std::invalid_argument(
                    "Initial vector contains non-finite values");
            }

            if (!ls_resolver)
            {
                throw std::invalid_argument(
                    "Linear solver is not defined");
            }

            IterationResult result;
            result.solution = init;

            Vector x = init;
            Vector fx = evaluate(system, x);

            if (!is_finite(fx))
            {
                result.status = ConvergenceStatus::NON_FINITE_VALUE;
                return result;
            }

            double residual = norm(fx);
            result.residual_norm = residual;

            if (residual <= options.tolerance)
            {
                result.status = ConvergenceStatus::CONVERGED;
                return result;
            }

            for (std::size_t k = 0; k < options.max_iterations; ++k)
            {
                Matrix J;

                try
                {
                    J = jacobian(system, x, options.jacobian_step);
                }
                catch (const std::domain_error&)
                {
                    result.status = ConvergenceStatus::NON_FINITE_VALUE;
                    result.solution = x;
                    result.residual_norm = residual;
                    result.iterations = k;
                    return result;
                }

                Vector rhs = fx;

                for (double& value : rhs)
                {
                    value = -value;
                }

                Vector delta;

                try
                {
                    delta = ls_resolver(J, rhs);
                }
                catch (const std::exception&)
                {
                    result.status = ConvergenceStatus::SINGULAR_JACOBIAN;
                    result.solution = x;
                    result.residual_norm = residual;
                    result.iterations = k;
                    return result;
                }

                if (delta.size() != system.dimension || !is_finite(delta))
                {
                    result.status = ConvergenceStatus::NON_FINITE_VALUE;
                    result.solution = x;
                    result.residual_norm = residual;
                    result.iterations = k;
                    return result;
                }

                bool accepted = false;

                double lambda = 1.0;

                Vector candidate;

                Vector candidate_fx;

                double candidate_residual = residual;

                while (lambda >= options.min_damping)
                {
                    candidate = x;

                    for (std::size_t i = 0; i < system.dimension; ++i)
                    {
                        candidate[i] += lambda * delta[i];
                    }

                    if (is_finite(candidate))
                    {
                        candidate_fx = evaluate(system, candidate);

                        if (is_finite(candidate_fx))
                        {
                            candidate_residual = norm(candidate_fx);

                            if (candidate_residual < residual)
                            {
                                accepted = true;
                                break;
                            }
                        }
                    }

                    lambda *= 0.5;
                }

                if (!accepted)
                {
                    result.solution = x;
                    result.residual_norm = residual;
                    result.iterations = k;

                    result.status = ConvergenceStatus::STAGNATION;
                    return result;
                }

                x = std::move(candidate);
                fx = std::move(candidate_fx);
                residual = candidate_residual;

                result.solution = x;
                result.residual_norm = residual;
                result.iterations = k + 1;

                if (residual <= options.tolerance)
                {
                    result.status = ConvergenceStatus::CONVERGED;
                    return result;
                }
            }

            result.solution = x;
            result.residual_norm = residual;
            result.status = ConvergenceStatus::MAX_ITERS;

            return result;
        }
}
