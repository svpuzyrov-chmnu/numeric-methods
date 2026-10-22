#include "matrix.hpp"
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>

namespace linear_algebra::matrix::simple
{
    Matrix make_matrix(const size_t& rows, const size_t& cols, const Generator& generator)
    {
        Matrix result(rows, Vector(cols));

        for (auto row = 0; row < rows; ++row)
        {
            for (auto column = 0; column < cols; ++column)
            {
                result.at(row).at(column) = generator(row, column);
            }
        }

        return result;
    }

    Matrix make_identity(const size_t& size)
    {
        auto g = [](const auto& row, const auto& col)
        {
            return row == col ? 1.0 : 0.0;
        };

        return make_matrix(size, size, g);
    }

    Vector operator-(const Vector& vector1, const Vector& vector2)
    {
        const auto size = std::min(vector1.size(), vector2.size());

        Vector result(size);

        for (size_t i = 0; i < size; ++i)
        {
            result.at(i) = vector1.at(i) - vector2.at(i);
        }

        return result;
    }

    Matrix operator*(const Matrix& m1, const Matrix& m2)
    {
        const auto rows = m1.size();

        const auto cols = m2.at(0).size();

        if (m1.at(0).size() != m2.size())
        {
            throw std::invalid_argument("Matrix first column count does not match with matrix second rows");
        }

        Matrix result(rows, Vector(cols));

        for (size_t row = 0; row < rows; ++row)
        {
            for (size_t col = 0; col < cols; ++col)
            {
                auto sum = 0.0;

                for (size_t k = 0; k < m2.size(); ++k)
                {
                    sum += m1.at(row).at(k) * m2.at(k).at(col);
                }

                result.at(row).at(col) = sum;
            }
        }

        return result;
    }

    Vector operator*(const Matrix& matrix, const Vector& vector)
    {
        if (matrix.size() != vector.size())
        {
            throw std::invalid_argument("matrix size does not match vector size");
        }

        Vector result(vector.size());

        for (size_t row = 0; row < vector.size(); ++row)
        {
            result.at(row) = 0.0;

            for (auto column = 0; column < matrix.size(); ++column)
            {
                result.at(row) += matrix.at(row).at(column) * vector.at(column);
            }
        }

        return result;
    }
}
