#include "matrix.hpp"
#include "exception.hpp"

namespace linear_algebra::matrix {
    void Matrix::check_indices(const size_t& i, const size_t& j) const
    {
        if (i < 0 || i >= data_.size())
        {
            throw exception::xInvalidIndex(i);
        }

        if (j < 0 || j >= data_.at(0)->size())
        {
            throw exception::xInvalidIndex(j);
        }
    }

    void Matrix::change_rows(const size_t& source_row, const size_t& target_row)
    {
        check_indices(source_row, 0);
        check_indices(target_row, 0);

        if (source_row != target_row)
        {
            const auto source_row_ref = data_.at(source_row);
            const auto target_row_ref = data_.at(target_row);

            data_.at(source_row) = target_row_ref;

            data_.at(target_row) = source_row_ref;
        }
    }

    bool UpperTriangleMatrix::is_over_defined(const size_t& r, const size_t& c) const
    {
        return r > c;
    }

    bool LowerTriangleMatrix::is_over_defined(const size_t& r, const size_t& c) const
    {
        return c > r;
    }
}
