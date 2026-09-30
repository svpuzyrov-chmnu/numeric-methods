#include "matrix-app.hpp"

void process_vectors()
{
    using std::cout;
    using std::endl;
    using linear_algebra::vector::Vector;

    Vector v1 {-1, 2.0, 4.5};
    Vector v2 {2, -13.8, 5.1};

    cout << "v1 = " << v1 << endl;
    cout << "v2 = " << v2 << endl;
    cout << "v1 + v2 = " << v1 + v2 << endl;
    cout << "v1 - v2 = " << v1 - v2 << endl;
    cout << "v1 * v2 = " << v1 * v2 << endl;
    cout << "v1 * 5 = " << v1 * 5 << endl;


}

void process_matrix()
{
    using linear_algebra::matrix::RectangleMatrix;

   double d[3][4] = {
        {-1.0, 2.3, 5.4, 7.2},
        {2.5, -5.1, 7.3, 4.1},
        {-0.1, 2.4, 11.8, 2.07}
    };

    RectangleMatrix m1 = d;

    auto m2 = m1 * 3.4;

    std::cout << std::endl << m1 << std::endl;
    std::cout << std::endl << m2 << std::endl;
}
