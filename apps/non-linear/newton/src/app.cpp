#include "app.hpp"

#include <functional>
#include "gauss.hpp"
#include "non-linear-newton.hpp"

#include <iostream>

void process_line_eq_sys_by_gauss()
{
    using nonlinear::core::ConvergenceStatus;
    using nonlinear::newton::IterationResult;
    using nonlinear::newton::LinearResolver;
    using nonlinear::newton::NonlinearSystem;
    using nonlinear::newton::Vector;
    using nonlinear::newton::resolve;

    const LinearResolver linear_resolver = [](const auto& jacobian, const auto& rhs)
    {
        return linear_algebra::gauss::simple::resolve(jacobian, rhs);
    };

    const auto print_vector = [](const Vector& vector)
    {
        std::cout << '(';
        for (std::size_t i = 0; i < vector.size(); ++i)
        {
            if (i != 0)
            {
                std::cout << ", ";
            }
            std::cout << vector[i];
        }
        std::cout << ')';
    };

    const auto status_name = [](ConvergenceStatus status)
    {
        switch (status)
        {
        case ConvergenceStatus::CONVERGED:
            return "converged";
        case ConvergenceStatus::MAX_ITERS:
            return "maximum iterations reached";
        case ConvergenceStatus::SINGULAR_JACOBIAN:
            return "singular Jacobian";
        case ConvergenceStatus::STAGNATION:
            return "stagnation";
        case ConvergenceStatus::NON_FINITE_VALUE:
            return "non-finite value";
        }

        return "unknown";
    };

    const auto print_result = [&print_vector, &status_name](
        const Vector& initial, const IterationResult& result)
    {
        std::cout << "Initial guess: ";
        print_vector(initial);
        std::cout << "\nSolution: ";
        print_vector(result.solution);
        std::cout << "\nResidual norm: " << result.residual_norm
                  << "\nIterations: " << result.iterations
                  << "\nStatus: " << status_name(result.status) << '\n';
    };

    const NonlinearSystem system_2{
        2,
        [](const Vector& x)
        {
            return Vector{
                x[0] * x[0] + x[1] * x[1] - 4.0,
                x[0] - x[1]};
        }};

    std::cout << "Newton method: 2-variable nonlinear system\n"
              << "  x^2 + y^2 - 4 = 0\n"
              << "  x - y = 0\n";
    const Vector initial_2{1.0, 1.0};

    const auto result_2 = resolve(system_2, initial_2, linear_resolver);

    print_result(initial_2, result_2);

    std::cout << '\n';

    const NonlinearSystem system_3{
        3,
        [](const Vector& x)
        {
            return Vector{
                x[0] * x[0] + x[1] * x[1] + x[2] * x[2] - 14.0,
                x[0] + x[1] + x[2] - 6.0,
                x[0] - x[1] + x[2] - 2.0};
        }};

    std::cout << "Newton method: 3-variable nonlinear system\n"
              << "  x^2 + y^2 + z^2 - 14 = 0\n"
              << "  x + y + z - 6 = 0\n"
              << "  x - y + z - 2 = 0\n";
    const Vector initial_3{0.8, 2.1, 3.1};
    const auto result_3 = resolve(system_3, initial_3, linear_resolver);
    print_result(initial_3, result_3);
}
