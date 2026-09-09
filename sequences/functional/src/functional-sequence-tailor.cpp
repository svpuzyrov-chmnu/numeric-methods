#include "functional-sequence-teilor.hpp"

#include <cmath>


namespace math_helpers::logarithm {
    static double multiplier(const double &x, const int &n) {
        return x * x * (2.0 * n + 1.0) / (2.0 * n + 3.0);
    }

    static double start_value(const double &x) {
        return x;
    }

    static double transform_to(const double &x) {
        using std::fabs;

        if (fabs(x) < 1.0) {
            return x;
        }

        return (x - 1) / (x + 1);
    }

    /**
 * Here is used fast logarithm sequence
 *    1+x
 * log------ = 2 * (x+x^3/3+x^5/5+x^7/7+...)
 *    1-x
 *  Internally argument x = t => (1+x)/(1-x) = t => x = (t-1)/(t+1)
 * @param x - argument
 * @param tolerance - tolerance of the computing
 * @return
 */
    double LogarithmFunctionalSequence::compute(const double &x, const double &tolerance) {
        using std::fabs;

        //transform argument

        const auto x1 = transform_to(x);

        auto current_term = start_value(x1);

        auto result = current_term;

        for (auto n = this->start_number; fabs(current_term) > tolerance; n++) {
            current_term *= multiplier(x1, n);
            result += current_term;
            ++(this->count_iterations_);
        }

        return 2*result;
    }
}

namespace math_helpers::exponential {
    static double multiplier(const double &x, const int &n) {
        return x / (n + 1.0);
    }

    static double start_value(const double &x) {
        return 1.0;
    }

    double ExponentialFunctionalSequence::compute(const double &x, const double &tolerance) {
        using std::fabs;


        auto current_term = start_value(x);

        auto result = current_term;

        for (auto n = this->start_number; fabs(current_term) > tolerance; n++) {
            current_term *= multiplier(x, n);
            result += current_term;
            ++(this->count_iterations_);
        }

        return result;
    }
}