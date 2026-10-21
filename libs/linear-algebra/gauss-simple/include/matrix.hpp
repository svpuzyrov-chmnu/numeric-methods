#pragma once

#include <iostream>
#include <vector>
#include <functional>

namespace linear_algebra::matrix::simple
{
    using Matrix = std::vector<std::vector<double>>;

    using Vector = std::vector<double>;

    using Generator = std::function<double(const size_t&, const size_t&)>;

    Matrix make_matrix(const size_t&, const size_t&, const Generator&);

    Matrix make_identity(const size_t&);

    std::ostream& operator<<(std::ostream&, const Matrix&);

    std::ostream& operator<<(std::ostream&, const Vector&);

    Vector operator-(const Vector&, const Vector&);

    Matrix operator*(const Matrix&, const Matrix&);

    Vector operator*(const Matrix&, const Vector&);
}
