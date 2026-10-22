#include "matrix-reader.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

namespace linear_algebra::matrix::simple::reader
{
    namespace validation
    {
        static void check_finite(const double& value, const std::size_t& line_number)
        {
            if (!std::isfinite(value))
            {
                throw std::runtime_error(
                    "Non-finite value at line " +
                    std::to_string(line_number));
            }
        }

        static void check_valid_line(
            const std::istringstream& ss,
            std::size_t line_number)
        {
            if (!ss.eof())
            {
                throw std::runtime_error(
                    "Invalid number at line " +
                    std::to_string(line_number));
            }
        }

        static void check_row_size(const std::size_t& size,
                                   const std::size_t& expected_size,
                                   const std::size_t& line_number)
        {
            if (size < expected_size)
            {
                throw std::runtime_error(
                    "Expected at least two numbers at line " +
                    std::to_string(line_number));
            }
        }

        static void check_column_count(
            const std::size_t& actual,
            const std::size_t& expected,
            const std::size_t& line_number)
        {
            if (actual != expected)
            {
                throw std::runtime_error(
                    "Inconsistent number of columns at line " +
                    std::to_string(line_number));
            }
        }

        static void check_stream(const std::istream& is)
        {
            if (is.bad())
            {
                throw std::runtime_error(
                    "I/O error while reading input");
            }
        }

        static void check_file_open(const std::ifstream& file, const std::string& filename)
        {
            if (!file.is_open())
            {
                throw std::runtime_error(
                    "Cannot open file: " + filename);
            }
        }
    }

    Vector read_vector(std::istream& is)
    {
        std::string line;
        std::getline(is, line);

        std::istringstream ss(line);

        Vector row;

        double item;

        while (ss >> item)
        {
            validation::check_finite(item, 1);
            row.push_back(item);
        }

        return row;
    }

    Vector read_vector(const std::string& filename)
    {
        std::ifstream file(filename);
        validation::check_file_open(file, filename);

        return read_vector(file);
    }

    Matrix read_matrix(std::istream& is)
    {
        Matrix result;
        std::string line;
        std::size_t line_number = 0;
        std::size_t columns = 0;

        while (std::getline(is, line))
        {
            ++line_number;

            std::istringstream ss(line);
            Vector row;
            double item;

            while (ss >> item)
            {
                validation::check_finite(item, line_number);
                row.push_back(item);
            }

            validation::check_valid_line(ss, line_number);

            if (row.empty())
            {
                continue;
            }

            validation::check_row_size(row.size(), 1, line_number);

            if (columns == 0)
            {
                columns = row.size();
            }
            else
            {
                validation::check_column_count(
                    row.size(), columns, line_number);
            }

            result.emplace_back(row.begin(), row.end());
        }

        validation::check_stream(is);

        return result;
    }

    Matrix read_matrix(const std::string& filename)
    {
        std::ifstream file(filename);
        validation::check_file_open(file, filename);

        return read_matrix(file);
    }

    ExtendMatrix read_ext_matrix(std::istream& is)
    {
        ExtendMatrix result;
        std::string line;
        std::size_t line_number = 0;
        std::size_t columns = 0;

        while (std::getline(is, line))
        {
            ++line_number;

            std::istringstream ss(line);
            Vector row;
            double item;

            while (ss >> item)
            {
                validation::check_finite(item, line_number);
                row.push_back(item);
            }

            validation::check_valid_line(ss, line_number);

            if (row.empty())
            {
                continue;
            }

            validation::check_row_size(row.size(), 2, line_number);

            if (columns == 0)
            {
                columns = row.size();
            }
            else
            {
                validation::check_column_count(
                    row.size(), columns, line_number);
            }

            result.matrix.emplace_back(
                row.begin(), row.end() - 1);

            result.vector.push_back(row.back());
        }

        validation::check_stream(is);

        return result;
    }

    ExtendMatrix read_ext_matrix(const std::string& filename)
    {
        std::ifstream file(filename);
        validation::check_file_open(file, filename);

        return read_ext_matrix(file);
    }

    std::istream& operator>>(std::istream& is, Matrix& m)
    {
        const auto& result = read_matrix(is);

        m = result;

        return is;
    }

    std::istream& operator>>(std::istream& is, ExtendMatrix& m)
    {
        const auto& result = read_ext_matrix(is);

        m = result;

        return is;
    }

    std::istream& operator>>(std::istream& is, Vector& v)
    {
        const auto& result = read_vector(is);

        v = result;

        return is;
    }
}
