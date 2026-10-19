#pragma once

#include "equation-core.hpp"
#include "equation-bisect.hpp"
#include "input-data.hpp"

using std::cout;
using std::endl;
using equation::core::Range;
using equation::core::ResultType;
using equation::core::Result;
using equation::core::InputData;
using equation::core::IEquationResolver;
using equation::bisect::BisectResolver;

namespace chord_app
{
    void process_equations();
}
