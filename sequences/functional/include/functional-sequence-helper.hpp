#pragma once

#include "functional-sequence-type.hpp"
#include "functional-sequence.hpp"

namespace math_helpers {

    class FunctionalSequenceHelper: public virtual FunctionalSequence {
        const function1_t start_value_evaluator_;
        const functional_sequence_term_multiplier_t multiplier_;
        const int start_number;
        int count_iterations_;

    public:
        FunctionalSequenceHelper(const function1_t start_value_evaluator,const functional_sequence_term_multiplier_t multiplier, const int &start_number = 0)
            : start_value_evaluator_(start_value_evaluator)
              , multiplier_(multiplier)
              , start_number(start_number)
              , count_iterations_(0) {
        }

        ~FunctionalSequenceHelper() override = default;

        [[nodiscard]] int count_iterations() const {
            return count_iterations_;
        }

        [[nodiscard]] double compute(const double &x, const double &tolerance = 1e-3) override;
    };
}
