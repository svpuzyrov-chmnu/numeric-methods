#pragma once

#include "functional-sequence.hpp"


namespace math_helpers::logarithm {
    class LogarithmFunctionalSequence : public virtual FunctionalSequence {
        const int start_number;
        int count_iterations_;

    public:
        LogarithmFunctionalSequence(const int &start_number = 0)
            : start_number(start_number)
              , count_iterations_(0) {
        }

        ~LogarithmFunctionalSequence() override = default;

        [[nodiscard]] int count_iterations() const override {
            return count_iterations_;
        }

        [[nodiscard]] double compute(const double &x, const double &tolerance = 1e-3) override;
    };
}

namespace math_helpers::exponential {
    class ExponentialFunctionalSequence : public virtual FunctionalSequence {
        const int start_number;
        int count_iterations_;

    public:
        ExponentialFunctionalSequence(const int &start_number = 0)
            : start_number(start_number)
              , count_iterations_(0) {
        }

        ~ExponentialFunctionalSequence() override = default;

        [[nodiscard]] int count_iterations() const override {
            return count_iterations_;
        }

        [[nodiscard]] double compute(const double &x, const double &tolerance = 1e-3) override;
    };
}