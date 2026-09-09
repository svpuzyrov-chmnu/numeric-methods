#include <iostream>
#include <iomanip>
#include <cmath>
#include "functional-sequence-helper.hpp"
#include "functional-sequence-teilor.hpp"
#include "functional-sequnce-app.hpp"

void enter_point(const double &a, const double &b, double &x) {
    do {
        std::cout << "Enter point between [" << a << ", " << b << "]: ";
        std::cin >> x;
    } while (x < a || x > b);
}

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

double multiplier(const double &x, const int &n) {
    const auto r = (1 + x) * (1.0 + x);
    return -1.0 * n / (n + 1.0) * r;
}

double start_evaluator(const double &x) {
    return -1.0 * (1 + x) * (1.0 + x);
}

double source_function(const double &x) {
    return -std::log(x * x + 2 * x + 2);
}

void process_compute() {
    do {
        int n = 1;

        double x;
        enter_point(-1.0, 0.1, x);

        double tolerance = 1e-4;
        enter_tolerance(tolerance);

        {
            std::cout << "Custom function sequence approximation: " << std::endl;

            auto helper = math_helpers::sequences::functional::FunctionalSequenceHelper(start_evaluator, multiplier, n);
            const auto result = helper.compute(x, tolerance);
            std::cout << std::setprecision(5) << "Tolerance: " << tolerance << std::endl;
            std::cout << "Quantity of terms: " << helper.count_iterations() << std::endl;
            std::cout << std::setprecision(10) << "Sum of series: " << result << std::endl;
            std::cout << std::setprecision(10) << "Value of source function: " << source_function(x) << std::endl;
        }

        {
            std::cout << "Fast logarithm approximation: " << std::endl;

            enter_point(0.001, 100.0, x);

            math_helpers::sequences::functional::logarithm::LogarithmFunctionalSequence helper;
            const auto result = helper.compute(x, tolerance);

            std::cout << std::setprecision(5) << "Tolerance: " << tolerance << std::endl;
            std::cout << "Quantity of terms: " << helper.count_iterations() << std::endl;
            std::cout << std::setprecision(10) << "Sum of series: " << result << std::endl;
            std::cout << std::setprecision(10) << "Value of source function: " << std::log(x) << std::endl;
        }

        {
            std::cout << "Exponential approximation: " << std::endl;

            enter_point(-100, 100.0, x);

            math_helpers::sequences::functional::exponential::ExponentialFunctionalSequence helper;
            const auto result = helper.compute(x, tolerance);

            std::cout << std::setprecision(5) << "Tolerance: " << tolerance << std::endl;
            std::cout << "Quantity of terms: " << helper.count_iterations() << std::endl;
            std::cout << std::setprecision(10) << "Sum of series: " << result << std::endl;
            std::cout << std::setprecision(10) << "Value of source function: " << std::exp(x) << std::endl;
        }


    } while (check_continue());
}
