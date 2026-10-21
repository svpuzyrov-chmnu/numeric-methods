#include "gauss.hpp"

namespace linear_algebra::gauss::simple
{
    static void check_matrix_square(const Matrix& m)
    {
        if (m.size() != m.at(0).size())
        {
            throw xNonSquareMatrix();
        }
    }

    static void check_infinite(const double& value)
    {
        if (!std::isfinite(value))
        {
            throw xInfiniteValue{};
        }
    }

    static void check_row_infinite(const Vector& v, const size_t& row)
    {
        for (size_t i = row; i < v.size(); ++i)
        {
            check_infinite(v.at(i));
        }
    }

    static size_t find_pivot_row(const Matrix& m, const size_t& row)
    {
        auto row_max_index = row;

        check_infinite(m.at(row_max_index).at(row));

        for (auto i = row + 1; i < m.size(); ++i)
        {
            check_infinite(m.at(i).at(row));
            if (std::fabs(m.at(i).at(row)) > std::fabs(m.at(row_max_index).at(row)))
            {
                row_max_index = i;
            }
        }

        return row_max_index;
    }

    static void change_row(Matrix& m, const size_t& source_row, const size_t& target_row)
    {
        for (size_t column = 0; column < m.size(); column++)
        {
            std::swap(
                m.at(source_row).at(column),
                m.at(target_row).at(column)
            );
        }
    }

    Vector resolve(Matrix m, Vector rhs)
    {
        if (m.empty() && rhs.empty())
        {
            return Vector{};
        }

        check_matrix_square(m);

        if (m.size() != rhs.size())
        {
            throw xMismatchRightSideSize();
        }

        const auto& size = m.size();

        auto check_solution = [&](const size_t& row) -> void
        {
            if (std::fabs(m.at(row).at(row)) < 1e-12)
            {
                if (std::fabs(rhs.at(row)) < 1e-12)
                {
                    throw xInfiniteSolution();
                }
                throw xNoSolution();
            }
        };

        //Forward step
        for (size_t row = 0; row < size; ++row)
        {
            check_row_infinite(m.at(row), row);

            check_infinite(rhs.at(row));

            const auto& pivot_row = find_pivot_row(m, row);

            if (pivot_row > row)
            {
                check_infinite(rhs.at(pivot_row));

                change_row(m, row, pivot_row);

                std::swap(rhs.at(row), rhs.at(pivot_row));
            }

            check_solution(row);

            for (auto next_row = row + 1; next_row < size; ++next_row)
            {
                const auto& factor = m.at(next_row).at(row) / m.at(row).at(row);

                for (auto col = row; col < size; ++col)
                {
                    m.at(next_row).at(col) -= m.at(row).at(col) * factor;
                }

                rhs.at(next_row) -= rhs.at(row) * factor;
            }
        }

        check_solution(size - 1);

        //Backward step
        Vector result(size);

        for (auto row = size; row-- > 0;)
        {
            auto sum = 0.0;

            for (auto i = row + 1; i < size; ++i)
            {
                sum += m.at(row).at(i) * result.at(i);
            }

            result.at(row) = (rhs.at(row) - sum) / m.at(row).at(row);
        }

        return result;
    }

    double det(Matrix m)
    {
        const auto& size = m.size();

        auto sign = 1;

        //Forward step
        for (size_t row = 0; row < size; ++row)
        {
            const auto& pivot_row = find_pivot_row(m, row);

            if (pivot_row > row)
            {
                change_row(m, row, pivot_row);
                sign = -sign;
            }

            for (auto i = row + 1; i < size; ++i)
            {
                const auto& factor = m.at(i).at(row) / m.at(row).at(row);

                for (auto j = row; j < size; ++j)
                {
                    m.at(i).at(j) -= m.at(row).at(j) * factor;
                }
            }
        }

        auto result = 1.0;

        for (size_t row = 0; row < size; ++row)
        {
            result *= m.at(row).at(row);
        }

        return result * sign;
    }

    Matrix inverse(Matrix m)
    {
        check_matrix_square(m);

        const auto& size = m.size();

        auto result = matrix::simple::make_identity(size);

        auto check_solution = [&](const size_t& row) -> void
        {
            if (std::fabs(m.at(row).at(row)) < 1e-12)
            {
                throw xInvMatrixNotExist{};
            }
        };

        //Forward step
        for (size_t row = 0; row < size; ++row)
        {
            check_infinite(m.at(row).at(row));

            const auto& pivot_row = find_pivot_row(m, row);

            if (pivot_row > row)
            {
                change_row(m, row, pivot_row);
                change_row(result, row, pivot_row);
            }

            check_solution(row);

            for (size_t next_row = row + 1; next_row < size; ++next_row)
            {
                const auto& factor = m.at(next_row).at(row) / m.at(row).at(row);

                for (size_t col = row; col < size; ++col)
                {
                    m.at(next_row).at(col) -= m.at(row).at(col) * factor;
                }

                for (size_t j = 0; j < size; ++j)
                {
                    result.at(next_row).at(j) -= result.at(row).at(j) * factor;
                }
            }
        }

        //Backward step
        for (size_t row = size; row-- > 0;)
        {
            for (size_t next_row = row; next_row-- > 0;)
            {
                const auto& factor = m.at(next_row).at(row) / m.at(row).at(row);

                m.at(next_row).at(row) = 0.0;

                for (size_t col = size; col-- > row;)
                {
                    m.at(next_row).at(col) -= factor * m.at(row).at(col);
                }

                for (size_t col = 0; col < size; ++col)
                {
                    result.at(next_row).at(col) -= result.at(row).at(col) * factor;
                }
            }
        }

        for (size_t row = size; row-- > 0;)
        {
            const auto& divider = m.at(row).at(row);

            for (size_t col = 0; col < size; ++col)
            {
                result.at(row).at(col) /= divider;
            }

            m.at(row).at(row) = 1.0;
        }

        return result;
    }
}
