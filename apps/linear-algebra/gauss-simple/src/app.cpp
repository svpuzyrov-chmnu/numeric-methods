#include "app.hpp"
#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "matrix-reader.hpp"
#include "matrix-writer.hpp"
#include "gauss.hpp"

namespace linear_algebra::gauss::simple::example
{
    using matrix::simple::reader::operator>>;
    using matrix::simple::writer::operator<<;
    using matrix::simple::operator*;
    using matrix::simple::operator-;
    using matrix::simple::reader::read_matrix;
    using matrix::simple::reader::read_ext_matrix;

    static void analise_linear_system(const Matrix& coefficients, const Vector& rhs)
    {
        using std::cout;
        using std::endl;

        cout << "****** Resolve linear eq system ******" << endl;

        const auto solution = resolve(coefficients, rhs);

        cout << "++++++ Coefficients" << endl << coefficients << endl;

        cout << "++++++ Right hand" << endl << rhs << endl;

        cout << "++++++ Solution" << endl << solution << endl;

        const auto back_rhs = coefficients * solution;

        cout << "++++++ Validate [A*x=b]" << endl << back_rhs << endl;

        cout << "++++++ Deviate" << endl << rhs - back_rhs << endl;

        cout << "++++++ Matrix determinant" << endl;

        cout << det(coefficients) << endl;

        cout << "++++++ Inverse of matrice" << endl;

        const auto& inv = inverse(coefficients);

        cout << inv << endl;
    }

    static void analise_linear_system(const matrix::simple::ExtendMatrix& em)
    {
        analise_linear_system(em.matrix, em.vector);
    }

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

        analise_linear_system(coefficients, rhs);

        constexpr auto file_name = "../../resources/matrix/system-2.1.txt";

        {
            cout << "===== Try to read matrix from file ======" << endl;

            if (std::ifstream file(file_name); file)
            {
                const auto& matrix = read_matrix(file);

                cout << matrix << endl;
            }
            else
            {
                cout << "Could not open file " << file_name << endl;
            }
        }

        {
            cout << "===== Try to read extend matrix from file ======" << endl;

            if (std::ifstream file(file_name); file)
            {
                matrix::simple::ExtendMatrix data;

                file >> data;

                analise_linear_system(data);
            }
            else
            {
                cout << "Could not open file " << file_name << endl;
            }
        }
    }
}
