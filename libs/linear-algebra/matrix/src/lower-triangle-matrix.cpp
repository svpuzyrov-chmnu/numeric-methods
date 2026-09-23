#include "lower-triangle-matrix.hpp"

namespace linear_algebra::matrix {
    bool LowerTriangleMatrix::is_over_defined(const size_t& r, const size_t& c) const
    {
        return c > r;
    }
}
