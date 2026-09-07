#pragma once

namespace math_helpers {
	class NumberSequenceHelper{
		double source;
	public:
		NumberSequenceHelper(const double& s): source(s)
		{}
		
		virtual ~NumberSequenceHelper()= default;

		[[nodiscard]] double round_to_sig(const int&) const;
	};

	double round_to_signum_digits(const double&, const int&);
}