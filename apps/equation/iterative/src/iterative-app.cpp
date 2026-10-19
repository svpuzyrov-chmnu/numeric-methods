#include "iterative-app.hpp"

namespace iterative_eq_app
{
void process_linear_by_iterative()
{

    cout << "========Iterative equation root========" << endl;

    for (const auto& [description, equation, factor, range, tolerance, expected] : input_data)
    {
        auto resolver = IterativeResolver(factor, tolerance);

        cout << description << endl;
        cout << "Expected root: " << expected << " in range: " << range << endl;

        const auto value = resolver.resolve(equation, range);

        cout << "Computed root: " << value << " with precision: " << tolerance << endl;
        cout << "Count of iteration: " << resolver.count_of_iterations() << endl;

        cout << "--------------------" << endl;
    }
}
}
