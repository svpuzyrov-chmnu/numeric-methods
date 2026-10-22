#pragma once

#include <iostream>
#include <string>
#include "matrix.hpp"

namespace linear_algebra::matrix::simple::reader
{
    Vector read_vector(std::istream&);

    Vector read_vector(const std::string&);

    Matrix read_matrix(std::istream&);

    Matrix read_matrix(const std::string&);

    ExtendMatrix read_ext_matrix(std::istream&);

    ExtendMatrix read_ext_matrix(const std::string&);

    std::istream& operator>>(std::istream&, Matrix&);

    std::istream& operator>>(std::istream&, ExtendMatrix&);

    std::istream& operator>>(std::istream&, Vector&);

}
