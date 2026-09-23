#pragma once

namespace math_helpers::sequences::functional {
    class FunctionalSequence {
    public:
        FunctionalSequence() = default;

        virtual ~FunctionalSequence() = default;

        virtual double compute(const double &x, const double &tolerance = 1e-3) = 0;

        [[nodiscard]] virtual int count_iterations() const = 0;
    };
}
