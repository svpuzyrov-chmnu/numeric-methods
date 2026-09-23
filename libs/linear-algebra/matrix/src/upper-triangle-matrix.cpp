#include "upper-triangle-matrix.hpp"

namespace linear_algebra::matrix {

    bool UpperTriangleMatrix::is_over_defined(const size_t& r, const size_t& c) const
    {
        return r > c;
    }
}
