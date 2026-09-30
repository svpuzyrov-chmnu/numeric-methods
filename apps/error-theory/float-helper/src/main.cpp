#include <iostream>
#include <iomanip>
#include "float-helper-app.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::setprecision;

int main()
{
	const auto big_number = 123456789.012378253;

	const auto small_number = 0.000289898457879823992;

	const auto to_small_number = 7.47382e-9;

	int sign_digits;

	cout << "Enter signum digits:";
	cin >> sign_digits;

	process_rounding_to("Big number: ", big_number, sign_digits);

	process_rounding_to("Small number: ", small_number, sign_digits);

	process_rounding_to("To small number: ", to_small_number, sign_digits);

	return 0;
}