#pragma once

namespace math_helpers::sequence::numeric {
	typedef double (*multiplier_t)(const int&);

	class NumberSequenceHelper{
		const multiplier_t multiplier_;
		const double start_value;
		const int start_number;
		int count_iterations_;
	public:
		NumberSequenceHelper(const multiplier_t multiplier, const double& start_value, const int& start_number = 0)
		: multiplier_(multiplier)
		, start_value(start_value)
		, start_number(start_number)
		,count_iterations_(0)
		{}
		
		virtual ~NumberSequenceHelper()= default;

		[[nodiscard]] int count_iterations() const {
			return count_iterations_;
		}

		[[nodiscard]] double compute(const double& tolerance = 1e-3);
	};
}
