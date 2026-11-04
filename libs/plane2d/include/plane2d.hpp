#pragma once

#include <vector>
#include <iostream>

namespace plane2d
{
    struct Point
    {
        double x;
        double y;
    };

    using Points = std::vector<Point>;

    void validate(const Points&);

    std::ostream& operator<<(std::ostream&, const Point&);

    std::ostream& operator<<(std::ostream&, const Points&);

}
