#pragma once

namespace math_helpers::tolerance {
	class FloatHelper{
		double source;
	public:
		FloatHelper(const double& s): source(s)
		{}
		
		virtual ~FloatHelper()= default;

		[[nodiscard]] double round_to_sig(const int&) const;

		static int sign_digits_by(const double&);
	};

	double round_to_signum_digits(const double&, const int&);
}