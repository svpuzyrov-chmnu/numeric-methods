#include "vector.hpp"

namespace linear_algebra::vector {
	void Vector::check_indices(const size_t& i) const
	{
		if (i < 0 || i > size())
		{
			throw xInvalidIndex(i);
		}
	}

	Vector& Vector::operator+=(const Vector& rhs)
	{
		const auto count_iterations = std::min(rhs.size(), this->size());

		for (size_t i = 0; i < count_iterations; i++)
		{
			data_[i] += rhs.data_[i];
		}

		return *this;
	}

	Vector& Vector::operator-=(const Vector& rhs)
	{
		const auto count_iterations = std::min(rhs.size(), this->size());

		for (size_t i = 0; i < count_iterations; i++)
		{
			data_[i] -= rhs.data_[i];
		}

		return *this;
	}

	Vector& Vector::operator*=(const double& rhs)
	{
		for (size_t i = 0; i < size(); i++)
		{
			data_[i] *= rhs;
		}

		return *this;
	}

	Vector operator+(const Vector& a, const Vector& b)
	{
		const auto result_size = std::min(a.size(), b.size());

		Vector result(result_size);

		for (size_t i = 0; i < result_size; i++)
		{
			result[i] = a[i] + b[i];
		}

		return result;
	}

	Vector operator-(const Vector& a, const Vector& b)
	{
		const auto result_size = std::min(a.size(), b.size());

		Vector result(result_size);

		for (size_t i = 0; i < result_size; i++)
		{
			result[i] = a[i] - b[i];
		}

		return result;
	}

	double operator*(const Vector& a, const Vector& b)
	{
		const auto result_size = std::min(a.size(), b.size());

		double result = 0.0;

		for (size_t i = 0; i < result_size; i++)
		{
			result += a[i] * b[i];
		}

		return result;
	}

	Vector operator*(const Vector& a, const double& n)
	{

		Vector result(a.size());

		for (size_t i = 0; i < result.size(); i++)
		{
			result[i] = a[i]* n;
		}

		return result;
	}

	std::ostream& operator<<(std::ostream& os, const Vector& v)
	{
		os << "[";
		for (size_t i = 0; i < v.size(); i++)
		{
			os << v[i];
			if (i < v.size() - 1)
			{
				os << ", ";
			}
		}
		os << "]";
		return os;
	}
}
