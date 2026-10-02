#include "gauss-app.hpp"

void process_linear_by_gauss()
{
    using std::cout;
    using std::endl;
    using linear_algebra::matrix::RectangleMatrix;
    using linear_algebra::vector::Vector;
    using linear_algebra::gauss::PartialGaussResolver;
    using linear_algebra::gauss::PartialGaussDeterminantResolver;
    using linear_algebra::gauss::PartialGaussInverseMatrixResolver;

    constexpr double raw_m[4][4] = {
        {-27.1489, -27.0561, 4.2419, -44.6195},
        {2.4671, -40.5327, 39.1517, -35.3451},
        {43.1613, -45.291, -16.3577, -10.1397},
        {44.6162, 3.4236, 19.3524, -24.0845},
    };

    const RectangleMatrix coefficients = raw_m;

    const Vector rhs { 2.5603, -10.1311, 8.5549, 18.4019};

    cout << coefficients << endl;
    cout << rhs << endl;

    const PartialGaussResolver system_resolver;
    const PartialGaussDeterminantResolver determinant_resolver;
    const PartialGaussInverseMatrixResolver inverse_resolver;

    const auto solution = system_resolver.resolve(coefficients, rhs);

    cout << endl << "Solution:" << solution << endl;

    cout << endl << "Validate:" << coefficients * solution << endl;
   
    cout << endl << "Deviate:" << system_resolver.deviate(coefficients, solution, rhs) << endl;

    cout << endl << "Determinant:" << determinant_resolver.determinant(coefficients) << endl;

    const auto inverse_m = inverse_resolver.inverse(coefficients);

    cout << "Inverse matrix:" << endl << inverse_m  << endl;

    cout << "Product of source and inverse matrix from left to right or right to left should produce identity matrix" << endl;

    const auto identity = coefficients * inverse_m;

    cout << identity << endl;

}