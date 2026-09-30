#include "matrix.hpp"
#include <iomanip>
#include <cmath>

namespace linear_algebra::matrix
{
    std::ostream& operator<<(std::ostream& os, const IMatrix& m)
    {
        for (size_t i = 0; i < m.rows(); ++i)
        {
            for (size_t j = 0; j < m.cols(); ++j)
            {
                const auto to_view = std::fabs( m(i, j)) <= 1e-12 ? 0.0 : m(i, j);
                os << std::setw(10) << std::setprecision(5) << to_view;
            }
            os << std::endl;
        }
        return os;
    }
}