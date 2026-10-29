#include "interpolation-core.hpp"
#include <cmath>

namespace interpolation::core {
    void validate(const Points &points) {
        if (points.empty()) {
            throw std::invalid_argument("Empty dataset");
        }

        const auto &size = points.size();

        for (size_t i = 0; i < size; ++i) {
            auto [x, y] = points[i];

            if (!std::isfinite(x) || !std::isfinite(y)) {
                throw std::invalid_argument(
                    "Point coordinates must be finite"
                );
            }

            for (std::size_t j = i + 1; j < size; ++j) {
                if (points.at(i).x == points[j].x) {
                    throw std::invalid_argument(
                        "Interpolation nodes must be distinct"
                    );
                }
            }
        }
    }

    std::ostream &operator<<(std::ostream &os, const Point &p) {
        os << "(" << p.x << ";" << p.y << ")";

        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Points &points) {
        os << "[";

        for (const auto &p: points) {
            os << p << ",";
        }

        return os;
    }
}
