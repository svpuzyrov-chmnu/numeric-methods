#include "gauss-app.hpp"

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