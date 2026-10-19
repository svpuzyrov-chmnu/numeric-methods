#pragma once

#include "equation-core.hpp"

using equation::core::Range;
using equation::core::ResultType;
using equation::core::Result;
using equation::core::InputData;
using equation::core::IEquationResolver;

namespace iterative_eq_app
{
    const std::vector<InputData> input_data = {
        {
            "x^2-8",
            [](auto x)
            {
                return x * x - 8;
            },
            0.1,
            Range(-4, -1),
            1e-7,
            -2 * std::sqrt(2)
        },
        {
            "x^2-8",
            [](auto x)
            {
                return x * x - 8;
            },
            -0.1,
            Range(1, 4),
            1e-7,
            2 * std::sqrt(2)
        },
        {
            "ln(x)-1",
            [](auto x)
            {
                return std::log(x) - 1;
            },
            -0.1,
            Range(2, 3),
            1e-7,
            std::exp(1.0)
        },
        {
            "sin(x)-0.5",
            [](auto x)
            {
                return std::sin(x) - 0.5;
            },
            -0.1,
            Range(0, 1),
            1e-7,
            std::asin(0.5)
        },
        {
            "cos(x)-x",
            [](auto x)
            {
                return std::cos(x) - x;
            },
            0.1,
            Range(0, 1),
            1e-7,
            0.7390851332151607
        },
        {
            "exp(x)-5",
            [](auto x)
            {
                return std::exp(x) - 5;
            },
            -0.1,
            Range(1, 2),
            1e-7,
            std::log(5.0)
        },
        {
            "x^3-2x-5",
            [](auto x)
            {
                return x * x * x - 2 * x - 5;
            },
            -0.1,
            Range(2, 3),
            1e-7,
            2.0945514815423265
        },
        {
            "x^4-5x^2+4",
            [](auto x)
            {
                const auto x_squared = x * x;
                return x_squared * x_squared - 5 * x_squared + 4;
            },
            0.01,
            Range(0, 1.5),
            1e-7,
            1
        },
        {
            "x*exp(x)-1",
            [](auto x)
            {
                return x * std::exp(x) - 1;
            },
            -0.1,
            Range(0, 1),
            1e-7,
            0.5671432904097838
        },
        {
            "ln(x)+x-2",
            [](auto x)
            {
                return std::log(x) + x - 2;
            },
            -0.1,
            Range(1, 2),
            1e-7,
            1.5571455989976114
        },
        {
            "2(1-x^2)+2*ln(4-x^2)",
            [](auto x)
            {
                return 2 * (1 - x * x) + 2 * std::log(4 - x * x);
            },
            -0.2,
            Range(-1.9, -1),
            1e-7,
            -1.3386784410121337
        },
        {
            "2(1-x^2)+2*ln(4-x^2)",
            [](auto x)
            {
                return 2 * (1 - x * x) + 2 * std::log(4 - x * x);
            },
            0.1,
            Range(1, 1.9),
            1e-7,
            1.3386784410121337
        }
    };
}
