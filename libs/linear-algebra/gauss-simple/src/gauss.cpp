#include "gauss.hpp"
#include <cmath>

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
        auto found_row = row;

        check_infinite(m.at(found_row).at(row));

        for (auto i = row + 1; i < m.size(); ++i)
        {
            const auto& current_item = m.at(found_row).at(row);

            const auto& next_item = m.at(i).at(row);

            check_infinite(next_item);

            if (std::fabs(next_item) > std::fabs(current_item))
            {
                found_row = i;
            }
        }

        return found_row;
    }

    static void change_row(Matrix& m, const size_t& from, const size_t& to)
    {
        for (size_t c = 0; c < m.size(); c++)
        {
            std::swap(m.at(from).at(c), m.at(to).at(c));
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
        for (size_t r = 0; r < size; ++r)
        {
            check_row_infinite(m.at(r), r);

            check_infinite(rhs.at(r));

            const auto& pivot_row = find_pivot_row(m, r);

            if (pivot_row > r)
            {
                check_infinite(rhs.at(pivot_row));

                change_row(m, r, pivot_row);

                std::swap(rhs.at(r), rhs.at(pivot_row));
            }

            check_solution(r);

            for (auto next_r = r + 1; next_r < size; ++next_r)
            {
                const auto& d = m.at(r).at(r);

                const auto& f = m.at(next_r).at(r) / d;

                for (auto c = r; c < size; ++c)
                {
                    m.at(next_r).at(c) -= m.at(r).at(c) * f;
                }

                rhs.at(next_r) -= rhs.at(r) * f;
            }
        }

        check_solution(size - 1);

        //Backward step
        Vector solution(size);

        for (auto r = size; r-- > 0;)
        {
            auto s = 0.0;

            auto d = m.at(r).at(r);

            for (auto i = r + 1; i < size; ++i)
            {
                s += m.at(r).at(i) * solution.at(i);
            }

            solution.at(r) = (rhs.at(r) - s) / d;
        }

        return solution;
    }

    double det(Matrix m)
    {
        const auto& size = m.size();

        auto sign = 1;

        //Forward step
        for (size_t r = 0; r < size; ++r)
        {
            const auto& pivot_row = find_pivot_row(m, r);

            if (pivot_row > r)
            {
                change_row(m, r, pivot_row);
                sign = -sign;
            }

            for (auto i = r + 1; i < size; ++i)
            {
                const auto& d = m.at(r).at(r);

                const auto& factor = m.at(i).at(r) / d;

                for (auto c = r; c < size; ++c)
                {
                    m.at(i).at(c) -= m.at(r).at(c) * factor;
                }
            }
        }

        auto result = 1.0;

        for (size_t r = 0; r < size; ++r)
        {
            result *= m.at(r).at(r);
        }

        return result * sign;
    }

    Matrix inverse(Matrix m)
    {
        check_matrix_square(m);

        const auto& size = m.size();

        auto res = matrix::simple::make_identity(size);

        auto check_solution = [&](const size_t& row) -> void
        {
            if (std::fabs(m.at(row).at(row)) < 1e-12)
            {
                throw xInvMatrixNotExist{};
            }
        };

        //Forward step
        for (size_t r = 0; r < size; ++r)
        {
            check_infinite(m.at(r).at(r));

            const auto& pivot_row = find_pivot_row(m, r);

            if (pivot_row > r)
            {
                change_row(m, r, pivot_row);
                change_row(res, r, pivot_row);
            }

            check_solution(r);

            for (size_t next_r = r + 1; next_r < size; ++next_r)
            {
                const auto& d = m.at(r).at(r);

                const auto& f = m.at(next_r).at(r) / d;

                for (size_t c = r; c < size; ++c)
                {
                    m.at(next_r).at(c) -= m.at(r).at(c) * f;
                }

                for (size_t c = 0; c < size; ++c)
                {
                    res.at(next_r).at(c) -= res.at(r).at(c) * f;
                }
            }
        }

        //Backward step
        for (size_t r = size; r-- > 0;)
        {
            for (size_t next_r = r; next_r-- > 0;)
            {
                const auto& d = m.at(r).at(r);

                const auto& f = m.at(next_r).at(r) / d;

                m.at(next_r).at(r) = 0.0;

                for (size_t c = size; c-- > r;)
                {
                    m.at(next_r).at(c) -= f * m.at(r).at(c);
                }

                for (size_t c = 0; c < size; ++c)
                {
                    res.at(next_r).at(c) -= res.at(r).at(c) * f;
                }
            }
        }

        for (size_t r = size; r-- > 0;)
        {
            const auto& d = m.at(r).at(r);

            for (size_t col = 0; col < size; ++col)
            {
                res.at(r).at(col) /= d;
            }

            m.at(r).at(r) = 1.0;
        }

        return res;
    }
}
