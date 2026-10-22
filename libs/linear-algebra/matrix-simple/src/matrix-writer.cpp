#include "matrix-writer.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

namespace linear_algebra::matrix::simple::writer
{
    void write_to(std::ostream& os, const Vector& v)
    {
        for (const auto& item : v)
        {
            os << std::setw(15) << std::setprecision(5) << item;
        }
    }

    void write_to(std::ostream& os, const Matrix& m)
    {
        for (const auto& row : m)
        {
            for (const auto& item : row)
            {
                os << std::setw(15) << std::setprecision(5) << item;
            }

            os << std::endl;
        }
    }

    void write_to(const std::string& file_name, const Matrix& m)
    {
        if (std::ofstream file(file_name); file)
        {
            write_to(file, m);
        }
    }

    void write_to(std::ostream& os, const ExtendMatrix& em)
    {
        for (const auto& rhs : em.vector)
        {
            for (const auto& row : em.matrix)
            {
                write_to(os, row);
            }

            os << rhs << std::endl;
        }
    }

    std::ostream& operator<<(std::ostream& os, const Matrix& m)
    {
        write_to(os, m);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Vector& v)
    {
        write_to(os, v);

        return os;
    }
}
