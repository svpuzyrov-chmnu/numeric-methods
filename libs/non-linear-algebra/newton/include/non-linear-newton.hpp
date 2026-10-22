#pragma once

#include "non-linear-core.hpp"

namespace nonlinear::newton
{
    using linear_algebra::matrix::simple::Vector;
    using linear_algebra::matrix::simple::Matrix;

    using core::IterationOptions;
    using core::IterationResult;

    using core::NonlinearSystem;

    using core::LinearResolver;

    IterationResult resolve(
        const NonlinearSystem& system,
        const Vector& init,
        const LinearResolver& ls_resolver,
        const IterationOptions& options = {});
}
