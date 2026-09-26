#include "upper-triangle-matrix.hpp"

namespace linear_algebra::matrix {

    bool UpperTriangleMatrix::is_over_defined(const size_t& r, const size_t& c) const
    {
        return r > c;
    }

    void UpperTriangleMatrix::init_row(const size_t& row,
        const std::function<double(const size_t&, const size_t&)>& col_size_generator)
    {
        for (size_t col = row; col < cols(); ++col)
        {
            data_.at(row)->at(  stored_col_index(row, col)) = col_size_generator(row, col);
        }
    }
}
