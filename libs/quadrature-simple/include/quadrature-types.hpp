#pragma once

#include <functional>
#include <cmath>

namespace numeric::quadratures::simple
{
    using Function = std::function<double(double)>;

    class QuadratureOptions
    {

    public:
        QuadratureOptions(double, double, int, const Function&);

        [[nodiscard]] double from() const
        {
            return from_;
        }

        [[nodiscard]] double from_value() const
        {
            return f_(from_);
        }

        [[nodiscard]] double to() const
        {
            return to_;
        }

        [[nodiscard]] double to_value() const
        {
            return f_(to_);
        }

        [[nodiscard]] double length() const
        {
            return length_;
        }

        [[nodiscard]] double step() const
        {
            return step_;
        }

        [[nodiscard]] double next_value_at(int i) const;

        [[nodiscard]] double next_value_at(const std::function<double()>&) const;

        [[nodiscard]] int subdivisions() const
        {
            return subdivisions_;
        }

        [[nodiscard]] bool is_singular() const
        {
            return std::fabs(length_) <= 1e-12;
        }

    private:
        void validate() const;

        double from_;

        double to_;

        int subdivisions_;

        double step_;

        double length_;

        const Function& f_;
    };
}