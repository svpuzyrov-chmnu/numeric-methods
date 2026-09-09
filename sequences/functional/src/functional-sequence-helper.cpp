#include "functional-sequence-helper.hpp"

#include <cmath>

namespace math_helpers::sequences::functional {
	double FunctionalSequenceHelper::compute(const double& x,const double& tolerance)
	{
		using std::fabs;

		auto current_term = this->start_value_evaluator_(x);

		auto result = current_term;

		for (auto n = this->start_number; fabs(current_term) > tolerance; n++) {
			current_term *= this->multiplier_(x, n);
			result += current_term;
			++(this->count_iterations_);
		}

		return result;
	}
}
