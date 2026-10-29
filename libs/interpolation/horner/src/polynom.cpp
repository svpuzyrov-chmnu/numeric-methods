#include <utility>
#include <stdexcept>
#include "polynom.hpp"
#include "horner.hpp"

namespace polynomial {
    Polynomial::Polynomial(Coefficients coefficients)
        : coefficients_(std::move(coefficients))
    {
        if (this->coefficients_.empty()) {
            throw std::invalid_argument("Empty polynomial");
        }
    }

    double Polynomial::evaluate(const double &x) const {
        return horner::evaluate(this->coefficients_, x);
    }

    std::size_t Polynomial::degree() const {
        return this->coefficients_.size() - 1;
    }
}
