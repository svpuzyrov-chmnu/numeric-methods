#include "quadrature-types.hpp"
#include <cmath>
#include <stdexcept>

namespace numeric::quadratures::simple {

    QuadratureOptions::QuadratureOptions(double from, double to, int subdivisions, const Function& f)
            : from_(from),
              to_(to),
              subdivisions_(subdivisions),
              step_(0),
              length_(0),
              f_(f)
    {
        validate();
        length_ = to_ - from_;
        step_ = length_ / subdivisions_;
    }

    void QuadratureOptions::validate() const
    {
        if (!f_)
            throw std::invalid_argument("Function must not be empty");

        if (!std::isfinite(from_) || !std::isfinite(to_))
            throw std::invalid_argument("Interval bounds must be finite");

        if (from_ > to_)
            throw std::invalid_argument("Lower bound must be less or equal to upper bound");

        if (!std::isfinite(length_))
            throw std::invalid_argument("Interval width must be finite");

        if (subdivisions_ <= 0)
            throw std::invalid_argument("Subdivisions must be positive");
    }

    double QuadratureOptions::next_value_at(int i) const
    {
        const auto& x = from_ + step_ * i;

        return f_(x);
    }

    double QuadratureOptions::next_value_at(const std::function<double()>& x_func) const
    {
        const auto& x = x_func();

        return f_(x);
    }
} 