#pragma once
#include <cstddef>
#include <functional>
#include <vector>

namespace numeric::quadratures {
    using Function = std::function<double(double)>;

    struct Interval {
        double lower;
        double upper;
    };

    enum class QuadratureType {
        LeftRectangle,
        RightRectangle,
        MidpointRectangle,
        Trapezoid,
        Simpson,
        GaussLegendre
    };

    struct QuadratureOptions {
        std::size_t subdivisions = 100;
        std::size_t gaussOrder = 4;
    };

    struct QuadratureNode {
        double x;
        double weight;
    };

    struct QuadratureResult {
        double value = 0.0;
        std::size_t evaluations = 0;
        std::vector<QuadratureNode> nodes;
    };
} // namespace numeric::quadratures
