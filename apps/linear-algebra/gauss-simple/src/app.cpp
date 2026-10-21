#include "app.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

namespace linear_algebra::gauss::simple::example
{
    using matrix::simple::operator<<;
    using matrix::simple::operator*;
    using matrix::simple::operator-;
    using matrix::simple::read_from;

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

        // const Matrix coefficients = {
        //     {1.20, 3.40, -0.50, 1.00},
        //     {-2.50, -4.10, -2.7, -2.0},
        //     {10.30, -22.50, 11.20, 6.00},
        //     {-0.80, 12.60, 3.10, 8.20}
        // };

        const Vector rhs{2.5603, -10.1311, 8.5549, 18.4019};
        // const Vector rhs{-1.30, 3.00, 2.40, 5.20};

        cout << "===== Resolve linear eq system ======" << endl;

        const auto solution = resolve(coefficients, rhs);

        cout << "Coefficients" << endl << coefficients  << endl;

        cout << "Right hand" << endl << rhs  << endl;

        cout << "Solution" << endl << solution << endl;

        const auto back_rhs = coefficients * solution;

        cout << "Validate [A*x=b]" << endl << back_rhs  << endl;

        cout << "Deviate" << endl << rhs - back_rhs << endl;

        cout << "===== Matrix determinant ======" << endl;

        cout << det(coefficients) << endl;

        cout << "===== Inverse of matrice ======" << endl;

        const auto& inv = inverse(coefficients);

        cout << inv << endl;

        cout << "===== Try to read matrix from file ======" << endl;

        constexpr auto file_name = "../../resources/matrix/system-2.1.txt";

        if (std::ifstream file(file_name); file)
        {
            const auto& matrix = read_from(file);

            cout << matrix << endl;
        } else
        {
            cout << "Could not open file " << file_name << endl;
        }
    }
}
