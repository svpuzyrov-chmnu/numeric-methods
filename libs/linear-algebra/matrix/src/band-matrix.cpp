#include "band-matrix.hpp"

namespace linear_algebra::matrix::band
{
    vector::Vector operator*(const BandMatrix<double>& matrix, const vector::Vector& rhs)
    {
        if (matrix.size() != rhs.size())
        {
            throw std::invalid_argument("Matrix and vector sizes do not match.");
        }

        vector::Vector result(matrix.size());

        for (std::size_t row = 0; row < matrix.size(); ++row)
        {
            double sum = 0.0;

            for (std::size_t col = 0; col < matrix.size(); ++col)
            {
                sum += matrix(row, col) * rhs[col];
            }

            result[row] = sum;
        }

        return result;
    }
}
