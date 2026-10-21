#pragma once

#include <iostream>
#include <vector>
#include <functional>

using std::cout;
using std::cin;
using std::endl;

namespace localizer
{
    using Equation = std::function<double(const double&)>;

    class Range
    {
        double left_;
        double right_;
    public:
        Range(const double& left, const double& right)
            : left_(left),
              right_(right)
        {}

        [[nodiscard]] double left() const
        {
            return left_;
        }

        [[nodiscard]] double right() const
        {
            return right_;
        }

        Range map(const Equation& mapper) const
        {
            return Range {mapper(left_), mapper(right_) };
        }

        [[nodiscard]] bool is_sign_diff() const
        {
            return std::signbit(left_) != std::signbit(right_);
        }
    };

    using Ranges = std::vector<Range>;

    std::ostream& operator<<(std::ostream& os, const Range& range);

    Ranges find_all_ranges(const Equation&, const Range&, const double&);

    void process_equations();
}
