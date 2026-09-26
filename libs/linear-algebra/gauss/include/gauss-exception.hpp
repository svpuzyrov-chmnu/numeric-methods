#pragma once
#include <exception>

namespace linear_algebra::gauss::exception {
    struct xInfinitySolution: std::exception
    {};

    struct xNoSolution: std::exception
    {};
}
