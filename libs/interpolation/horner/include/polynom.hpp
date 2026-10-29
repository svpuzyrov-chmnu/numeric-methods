#pragma once

#include "polynom-core.hpp"

namespace polynomial {

    class Polynomial {
        Coefficients coefficients_;
    public:
        explicit Polynomial(Coefficients coefficients);

        [[nodiscard]] double evaluate(const double &x) const;

        double operator()(const double &x) const {
            return evaluate(x);
        }

        [[nodiscard]] std::size_t degree() const;

        [[nodiscard]] Coefficients coefficients() const
        {
            return this->coefficients_;
        }
    };
}
