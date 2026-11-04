#include "app.hpp"
#include <iostream>
#include "lagrange.hpp"

void process_lagrange_interpolation()
{
    using std::cout;
    using std::cin;
    using std::endl;

    using plane2d::Points;
    using interpolation::lagrange::evaluate;
    
    //v25
    const Points points{ {1.231, 0.839}, {1.684, 1.498}, {1.921, 1.781} , {2.136, 2.023} , {2.174, 2.066} , {2.867, 2.805}, {4.873, 4.852}, {6.312, 6.3} };

	cout << "Interpolation points: " << points << endl;

	cout << "Enter value to interpolate: ";
    double value;

	cin >> value;

    const double result = evaluate(points, value);

    cout << "Interpolated value: " << result << endl;

}