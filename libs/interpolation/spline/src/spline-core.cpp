#include "spline-core.hpp"

namespace interpolation::spline::core {
    Nodes create_from(const Points & points) {
        Nodes result;

        for (size_t i = 0; i < points.size(); ++i) {
            result.emplace_back(i, points[i]);
        }

        return result;
    }

}
