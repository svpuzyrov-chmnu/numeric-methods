#include <iostream>
#include <iomanip>
#include "float-helper.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::setprecision;

static void process_rounding_to(const char* msg, const double&, const int&);

int main()
{
	const auto big_number = 123456789.012378253;

	const auto small_number = 0.000289898457879823992;

	int sign_digits;

	cout << "Enter signum digits:";
	cin >> sign_digits;

	process_rounding_to("Big number: ", big_number, sign_digits);

	process_rounding_to("Small number: ", small_number, sign_digits);

	return 0;
}

static void process_rounding_to(const char* msg, const double& source, const int& digits)
{
	using math_helpers::round_to_signum_digits;

	const auto result = round_to_signum_digits(source, digits);

	cout << setprecision(12);

	cout << msg << result << endl;
}
