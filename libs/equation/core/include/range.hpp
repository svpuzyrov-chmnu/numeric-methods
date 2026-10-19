#pragma once

#include <iostream>
#include <functional>
#include <cmath>

namespace equation::core
{
    class Range
    {
        double left_;
        double right_;

    public:
        Range(const double& left, const double& right)
            : left_(left),
              right_(right)
        {
        }

        const double& left() const noexcept { return left_; }

        const double& right() const noexcept { return right_; }

        [[nodiscard]] double center() const noexcept { return left_ + (right_ - left_) / 2; }

        [[nodiscard]] Range left_center() const
        {
            return Range{left_, center()};
        }

        [[nodiscard]] Range right_center() const
        {
            return Range {center(), right_ };
        }

        Range map(const std::function<double(const double&)>& mapper) const
        {
            return Range {mapper(left_), mapper(right_) };
        }

        [[nodiscard]] bool length_is_lq(const double& tolerance) const
        {
            return std::fabs(right_ - left_) <= tolerance;
        }

        [[nodiscard]] double length() const
        {
            return right_ - left_;
        }

        [[nodiscard]] bool is_border(const double& x) const
        {
            return x == left_ || x == right_;
        }
    };

    std::ostream& operator<<(std::ostream&, const Range&);
}
