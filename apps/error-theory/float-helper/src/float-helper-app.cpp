#include "float-helper-app.hpp"

void process_rounding_to(const char* msg, const double& source, const int& digits)
{
    using math_helpers::tolerance::round_to_signum_digits;

    const auto result = round_to_signum_digits(source, digits);

    std::cout << std::setprecision(12);

    std::cout << msg << result << std::endl;
}
