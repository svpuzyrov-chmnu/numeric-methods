#pragma once

#include <cmath>

namespace math_helpers {
	class FloatHelper{
		double source;
	public:
		FloatHelper(const double& s): source(s)
		{}
		
		virtual ~FloatHelper()
		{}

		const double round_to_sig(const int&) const;
	};

	double round_to_signum_digits(const double&, const int&);
}