#include "number-sequence.hpp"

void enter_tolerance(double &tolerance) {
    do {
        std::cout << "Enter tolerance: ";
        std::cin >> tolerance;
    } while (tolerance < 0.0 || tolerance > 1.0);
}

bool check_continue() {
    char ch;
    std::cout << "Continue? (y/any key)\n";
    std::cin >> ch;

    return ch == 'y' || ch == 'Y';
}

double multiplier(const int &n) {
    return (n + 1.0) / 2.0 / (2.0 * n + 1.0);
}

void process_compute() {
    do {
        double start_value = 1.0;
        int n = 0;

        double tolerance = 1e-4;

        enter_tolerance(tolerance);

        auto helper = math_helpers::NumberSequenceHelper(multiplier, start_value, n);

        const auto result = helper.compute(tolerance);
        std::cout << std::setprecision(5) << "Tolerance: " << tolerance << std::endl;
        std::cout << "Quantity of terms: " << helper.count_iterations() << std::endl;
        std::cout << std::setprecision(10) << "Sum of series: " << result << std::endl;
    } while (check_continue());
}
