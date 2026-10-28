#include "horner.hpp"
#include <cmath>
#include <stdexcept>

namespace polynomial::horner
{
    static void validate(const Coefficients & c)
    {
        for (const auto & coeff : c) {
            if (!std::isfinite(coeff)) {
                throw std::invalid_argument("Coefficient must be finite");
            }
        }
    }

    double evaluate(const Coefficients & c, const double & x) {
        validate(c);

        if (c.empty()) {
            return 0.0;
        }

        auto result = 0.0;

        for (auto it = c.crbegin(); it != c.crend(); ++it)
        {
            result = result * x + *it;
        }

        return result;
    }
}
