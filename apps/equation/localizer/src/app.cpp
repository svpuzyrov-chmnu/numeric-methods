#include "app.hpp"

namespace localizer
{
    std::ostream& operator<<(std::ostream& os, const Range& range)
    {
        return os << "[" << range.left() << ", " << range.right() << "]";
    }

    Ranges find_all_ranges(const Equation& equation, const Range& range, const double& step)
    {
        std::vector<Range> result;

        for (auto x = range.left(); x < range.right(); x += step)
        {
            const auto& sub_range = Range{x, x + step};

            const auto& value_range = sub_range.map(equation);

            if (value_range.is_sign_diff())
            {
                result.push_back(sub_range);
            }
        }

        return result;
    }

    void process_equations()
    {
        cout << "========Find all root ranges ========" << endl;

        cout << "Enter start, end, step:";

        double start = 0.0, end = 0.0, step = 0.0;

        if (!(cin >> start >> end >> step))
        {
            cout << "Invalid input" << endl;
            return;
        }

        auto equation = [&](auto x)
        {
            return x * x * x - 3 * x * x - 4 * x + 1;
        };

        const auto& range = Range{start, end};

        const auto& ranges = find_all_ranges(equation, range, step);

        if (ranges.empty())
        {
            cout << "Range of roots was not found" << endl;
            return;
        }

        for (const auto& range : ranges)
        {
            cout << range << endl;
        }
    }
}
