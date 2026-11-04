#pragma once

#include <vector>
#include <iostream>
#include <functional>

namespace plane2d
{
    struct Point
    {
        double x;
        double y;

        Point() : x(0.0), y(0.0) {}

        Point(const double x, const double y)
            : x(x), y(y) {}
    };

    using Points = std::vector<Point>;

    void validate(const Points&);

    Points tabulate(const std::function<double(double)>&, double, double, std::size_t);

    Point left_point(const Points&);

    Point right_point(const Points&);

    std::ostream& operator<<(std::ostream&, const Point&);

    std::ostream& operator<<(std::ostream&, const Points&);

}
