#include "gauss-app.hpp"

void process_linear_by_gauss()
{
    using std::cout;
    using std::endl;
    using linear_algebra::matrix::RectangleMatrix;
    using linear_algebra::vector::Vector;
    using linear_algebra::gauss::PartialGaussResolver;

    double raw_m[4][4] = {
        {-27.1489, -27.0561, 4.2419, -44.6195},
        {2.4671, -40.5327, 39.1517, -35.3451},
        {43.1613, -45.291, -16.3577, -10.1397},
        {44.6162, 3.4236, 19.3524, -24.0845},
    };

    RectangleMatrix coefficients = raw_m;

    Vector right_side_v { 2.5603, -10.1311, 8.5549, 18.4019};

    cout << coefficients << endl;
    cout << right_side_v << endl;

    auto resolver = PartialGaussResolver{};

    auto solution = resolver.resolve(coefficients, right_side_v);

    cout << endl << "Solution:" << solution << endl;

    cout << endl << "Validate:" << coefficients * solution << endl;
   
    cout << endl << "Deviate:" << resolver.deviate(coefficients, solution, right_side_v) << endl;

    cout << endl << "Determinant:" << resolver.determinant(coefficients) << endl;
}