#pragma once

#include <string>
#include "plane2d.hpp"

namespace plot2d
{
    enum class CurveStyle {
        Line,
        Points,
        LineWithPoints
    };

    struct Curve {
        plane2d::Points points;
        std::string label;
        CurveStyle style = CurveStyle::Line;
        int lineWidth = 2;
    };

}
