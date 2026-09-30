#include "matrix.hpp"
#include <iomanip>

namespace linear_algebra::matrix
{
    std::ostream& operator<<(std::ostream& os, const IMatrix& m)
    {
        for (size_t i = 0; i < m.rows(); ++i)
        {
            for (size_t j = 0; j < m.cols(); ++j)
            {
                os << std::setw(10) << std::setprecision(5) << m(i, j);
            }
            os << std::endl;
        }
        return os;
    }
}