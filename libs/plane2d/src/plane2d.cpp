#include "plane2d.hpp"
#include <algorithm>
#include <cmath>

namespace plane2d {
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
                        "Points must be distinct"
                    );
                }
            }
        }
    }

    Points tabulate(const std::function<double(double)> &f, const double a, const double b, const std::size_t count) {
        Points points;

        points.reserve(count);

        const double step = (b - a) / static_cast<double>(count - 1);

        for (std::size_t i = 0; i < count; ++i) {
            const double x = a + step * static_cast<double>(i);
            const double y = f(x);

            points.emplace_back(x, y);
        }

        return points;
    }

    Point left_point(const Points & points) {
        if (points.empty()) {
            throw std::invalid_argument("Empty dataset");
        }

        const auto it = std::min_element(points.begin(), points.end(), [](const Point &a, const Point &b) {
            return a.x < b.x;
        });

        return *it;
    }

    Point right_point(const Points & points) {
        if (points.empty()) {
            throw std::invalid_argument("Empty dataset");
        }

        const auto it = std::max_element(points.begin(), points.end(), [](const Point &a, const Point &b) {
            return a.x < b.x;
        });

        return *it;
    }

    std::ostream &operator<<(std::ostream &os, const Point &p) {
        os << "(" << p.x << ";" << p.y << ")";

        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Points &points) {
        os << "[";

        for (std::size_t i = 0; i < points.size(); ++i) {
            os << points[i];

			if (i != points.size() - 1) {
				os << ", ";
			}
        }

		os << "]";

        return os;
    }
}
