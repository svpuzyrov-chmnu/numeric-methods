#include "lower-triangle-matrix.hpp"

namespace linear_algebra::matrix {
    bool LowerTriangleMatrix::is_over_defined(const size_t& r, const size_t& c) const
    {
        return c > r;
    }

    void LowerTriangleMatrix::init_row(const size_t& row, const std::function<double(const size_t& row, const size_t&)>& col_size_generator)
    {
        for (size_t col = 0; col <= row && col < cols(); ++col)
        {
            data_.at(row)->at(stored_col_index(row, col)) = col_size_generator(row, col);
        }
    }
}
