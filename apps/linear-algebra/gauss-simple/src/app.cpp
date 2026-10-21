#include "app.hpp"
#include <iostream>
#include <iomanip>

namespace linear_algebra::gauss::simple::example
{
    using matrix::simple::operator<<;
    using matrix::simple::operator*;
    using matrix::simple::operator-;

    void process_linear_systems_by_gauss()
    {
        using std::cout;
        using std::endl;

        const Matrix coefficients = {
            {-27.1489, -27.0561, 4.2419, -44.6195},
            {2.4671, -40.5327, 39.1517, -35.3451},
            {43.1613, -45.291, -16.3577, -10.1397},
            {44.6162, 3.4236, 19.3524, -24.0845}
        };

        const Vector rhs{2.5603, -10.1311, 8.5549, 18.4019};

        cout << "===== Resolve system of linear equations ======" << endl;

        const auto solution = resolve(coefficients, rhs);

        cout << "Coefficients" << endl << coefficients  << endl;

        cout << "Right hand" << endl << rhs  << endl;

        cout << "Solution" << endl << solution << endl;

        const auto back_rhs = coefficients * solution;

        cout << "Validate [A*x=b]" << endl << back_rhs  << endl;

        cout << "Deviate" << endl << rhs - back_rhs << endl;

        cout << "===== Find determinant of matrice ======" << endl;

        cout << det(coefficients) << endl;

        cout << "===== Find inverse of matrice ======" << endl;

        const auto& inv = inverse(coefficients);

        cout << inv << endl;
    }
}
