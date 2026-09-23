#include "rectangle-matrix.hpp"
#include "exception.hpp"

namespace linear_algebra::matrix {
    void RectangleMatrix::check_indices(const size_t& i, const size_t& j) const
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

    void RectangleMatrix::change_rows(const size_t& i, const size_t& j)
    {
        check_indices(i, 0);
        check_indices(j, 0);

        if (i != j)
        {
            const auto source_row_ref = data_.at(i);
            const auto target_row_ref = data_.at(j);

            data_.at(i) = target_row_ref;

            data_.at(j) = source_row_ref;
        }
    }

    void RectangleMatrix::change_cols(const size_t& i, const size_t& j)
    {
        check_indices(0, i);
        check_indices(0, j);

        if (i != j)
        {
            for (const auto & row_vector : data_)
            {
                std::swap(row_vector->at(i), row_vector->at(j));
            }
        }
    }
}
