#pragma once

#include "equation-core.hpp"
#include "equation-chord.hpp"
#include "input-data.hpp"

using std::cout;
using std::endl;
using equation::core::Range;
using equation::core::ResultType;
using equation::core::Result;
using equation::core::IEquationResolver;
using equation::iterative::IterativeResolver;

namespace iterative_eq_app
{
    void process_linear_by_iterative();
}
