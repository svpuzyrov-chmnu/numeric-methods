#pragma once

#include <iostream>

namespace linear_algebra::matrix
{
    class Matrix
    {
    public:
        Matrix() = default;

        virtual ~Matrix() = default;

        [[nodiscard]] virtual double at(const size_t& i, const size_t& j) const = 0;

        virtual double operator()(const size_t& i, const size_t& j) const { return at(i, j); }

        virtual double& at(const size_t& i, const size_t& j) = 0;

        virtual double& operator()(const size_t& i, const size_t& j)
        {
            return at(i, j);
        }

        virtual void change_rows(const size_t& i, const size_t& j)
        {}

        virtual void change_cols(const size_t& i, const size_t& j)
        {}

        [[nodiscard]] virtual const size_t rows() const = 0;

        [[nodiscard]] virtual const size_t cols() const = 0;
    };

    std::ostream& operator<<(std::ostream& os, const Matrix& m);
}
