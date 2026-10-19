#include "bisect-app.hpp"

namespace chord_app
{
    void process_equations()
    {
        cout << "========Bisect equation root========" << endl;

        for (const auto& [description, equation, _, range, tolerance, expected] : input_data)
        {
            auto resolver = BisectResolver(tolerance);

            cout << description << endl;
            cout << "Expected root: " << expected << " in range: " << range << endl;

            const auto value = resolver.resolve(equation, range);

            cout << "Computed root: " << value << " with precision: " << tolerance << endl;
            cout << "Count of iteration: " << resolver.count_of_iterations() << endl;

            cout << "--------------------" << endl;
        }
    }
}
