#include "float-helper.h"

namespace math_helpers {
	const double FloatHelper::round_to_sig(const int& digits) const
	{
		using std::fabs;
		using std::floor;
		using std::log10;
		using std::pow;
		using std::round;
		
		if (source == 0.0)
		{
			return 0.0;
		}

		auto abs_value = fabs(this->source);

		auto exponent = static_cast<int>(floor(log10(abs_value) + 1e-12));

		auto factor = pow(10.0, digits - 1 - exponent);

		return round(this->source * factor) / factor;
	}

	double round_to_signum_digits(const double& source, const int& sign_digits)
	{
		auto helper = FloatHelper(source);

		return helper.round_to_sig(sign_digits);
	}
}