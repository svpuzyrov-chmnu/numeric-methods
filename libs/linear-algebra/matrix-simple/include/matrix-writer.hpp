#pragma once

#include <iostream>
#include <string>
#include "matrix.hpp"

namespace linear_algebra::matrix::simple::writer
{
    void write_to(std::ostream&, const Vector&);

    void write_to(const std::string&, const Vector&);

    void write_to(std::ostream&, const Matrix&);

    void write_to(const std::string&, const Matrix&);

    void write_to(std::ostream&, const ExtendMatrix&);

    std::ostream& operator<<(std::ostream&, const Matrix&);

    std::ostream& operator<<(std::ostream&, const ExtendMatrix&);

    std::ostream& operator<<(std::ostream&, const Vector&);
}
