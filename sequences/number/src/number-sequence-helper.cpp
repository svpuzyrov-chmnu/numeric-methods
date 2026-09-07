#include "number-sequence-helper.hpp"

#include <cmath>

namespace math_helpers {
	double NumberSequenceHelper::compute(const double& tolerance)
	{
		using std::fabs;

		auto current_term = this->start_value;

		auto result = current_term;

		for (auto n = this->start_number; fabs(current_term) > tolerance; n++) {
			current_term *= this->multiplier_(n);
			result += current_term;
			++(this->count_iterations_);
		}

		return result;
	}
}
