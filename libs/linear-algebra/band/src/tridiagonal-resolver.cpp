#include "tridiagonal-resolver.hpp"

namespace linear_algebra::tridiagonal
{
    vector::Vector TridiagonalResolver::resolve(const matrix::band::BandMatrix<double>& matrix, const vector::Vector& rhs)
    {
        const auto size = matrix.size();

        if (matrix.lowerBandWidth() != 1 || matrix.upperBandWidth() != 1)
        {
            throw std::invalid_argument("Matrix must be tridiagonal");
        }

        if (rhs.size() != size)
        {
            throw std::invalid_argument("RHS size must match matrix size");
        }

        if (size == 0)
        {
            return {};
        }

        vector::Vector alpha(size);
        vector::Vector beta(size);

        // First row
        if (matrix(0, 0) == 0.0)
        {
            throw std::runtime_error("Zero pivot encountered");
        }

        if (size > 1)
        {
            alpha[0] = -matrix(0, 1) / matrix(0, 0);
        }

        beta[0] = rhs[0] / matrix(0, 0);

        // Forward sweep
        for (std::size_t i = 1; i < size; ++i)
        {
            const auto denominator = matrix(i, i) + matrix(i, i - 1) * alpha[i - 1];

            if (denominator == 0.0)
            {
                throw std::runtime_error("Zero pivot encountered");
            }

            if (i < size - 1)
            {
                alpha[i] = -matrix(i, i + 1) / denominator;
            }

            beta[i] = (rhs[i] - matrix(i, i - 1) * beta[i - 1]) / denominator;
        }

        // Back substitution
        vector::Vector x(size);

        x[size - 1] = beta[size - 1];

        for (std::size_t i = size - 1; i-- > 0;)
        {
            x[i] = alpha[i] * x[i + 1] + beta[i];
        }

        return x;
    }
}