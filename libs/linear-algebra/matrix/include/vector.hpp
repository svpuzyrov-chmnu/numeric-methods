#pragma once
#include <vector>
#include <initializer_list>
#include <iostream>
#include <functional>

namespace linear_algebra::vector {

    class Vector final {
        std::vector<double> data_;
        void check_indices(const size_t&) const;
    public:
        Vector() = default;

        Vector(const size_t& sz) : data_(sz) {}

        Vector(const size_t& sz, const std::function<double(const size_t&)>& init_func)
        : data_(sz)
        {
            for (size_t i = 0; i < sz; ++i)
            {
                data_[i] = init_func(i);
            }
        }

        Vector(const std::vector<double>& data) : data_(data) {}

        Vector(const std::initializer_list<double>& data) : data_(data) {}

        Vector(const double* data, const std::size_t& size) : data_(size)
        {
            for (int i = 0; i < size; i++)
            {
                data_.push_back(data[i]);
            }
        }

        template<int size>
        Vector(double (&data)[size]) : data_(size)
        {
            for (int i = 0; i < size; i++)
            {
                data_[i] = data[i];
            }
        }

        double operator[](const size_t& i) const
        {
            check_indices(i);
            return at(i);
        }

        double& operator[](const size_t& i) { return data_.at(i); }

        [[nodiscard]] double at(const size_t& i) const { return data_.at(i); }

        double& at(const size_t& i) { return data_.at(i); }

        [[nodiscard]] size_t size() const
        {
            return data_.size();
        }

        void clear()
        {
            data_.clear();
        }

        Vector& operator+=(const Vector&);
        Vector& operator-=(const Vector&);
        Vector& operator*=(const double&);

        friend Vector operator+(const Vector&, const Vector&);

        friend Vector operator-(const Vector&, const Vector&);

        friend double operator*(const Vector&, const Vector&);

        friend Vector operator*(const Vector&, const double&);
    };

    Vector operator+(const Vector&, const Vector&);

    Vector operator-(const Vector&, const Vector&);

    double operator*(const Vector&, const Vector&);

    Vector operator*(const Vector&, const double&);

    std::ostream& operator<<(std::ostream&, const Vector&);
}
