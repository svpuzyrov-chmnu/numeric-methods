#pragma once

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "vector.hpp"

namespace linear_algebra::matrix::band
{
    template <typename T>
    class BandMatrix
    {
    public:
        using size_type = std::size_t;

        BandMatrix(
            const size_type& size,
            const size_type& lowerBandwidth,
            const size_type& upperBandwidth)
            : size_(size),
              lowerBandwidth_(lowerBandwidth),
              upperBandwidth_(upperBandwidth),
              data_(size * (lowerBandwidth + upperBandwidth + 1), T{})
        {
        }

        BandMatrix(
            size_type size,
            const std::vector<std::vector<T>>& bands)
            : size_(size),
              lowerBandwidth_(bands.size() / 2),
              upperBandwidth_(bands.size() / 2),
              data_(size * bands.size(), T{})
        {
            const auto bandwidth = bands.size() / 2;

            for (size_type band = 0; band < bands.size(); ++band)
            {
                const auto diagonal =
                    static_cast<std::ptrdiff_t>(band)
                    - static_cast<std::ptrdiff_t>(bandwidth);

                const auto& values = bands[band];

                for (size_type i = 0; i < values.size(); ++i)
                {
                    const auto row = static_cast<std::ptrdiff_t>(i);

                    if (const std::ptrdiff_t column = row + diagonal; row >= 0 &&
                        column >= 0 &&
                        row < static_cast<std::ptrdiff_t>(size_) &&
                        column < static_cast<std::ptrdiff_t>(size_))
                    {
                        (*this)(
                            static_cast<size_type>(row),
                            static_cast<size_type>(column)
                        ) = values[i];
                    }
                }
            }
        }

        [[nodiscard]] const size_type& size() const
        {
            return size_;
        }

        [[nodiscard]] const size_type& lowerBandWidth() const
        {
            return lowerBandwidth_;
        }

        [[nodiscard]] const size_type& upperBandWidth() const
        {
            return upperBandwidth_;
        }

        T& operator()(const size_type& row, const size_type& column)
        {
            return data_.at(index(row, column));
        }

        const T& operator()(const size_type& row, const size_type& column) const
        {
            checkBounds(row, column);
            if (!isInsideBand(row, column))
            {
                return zero_;
            }

            return data_.at(index(row, column));
        }

        friend std::ostream& operator<<(std::ostream& os, const BandMatrix& matrix)
        {
            for (std::size_t row = 0; row < matrix.size_; ++row)
            {
                for (std::size_t column = 0; column < matrix.size_; ++column)
                {
                    if (column > 0)
                    {
                        os << ' ';
                    }

                    os << matrix(row, column);
                }

                os << '\n';
            }

            return os;
        }

    private:
        [[nodiscard]]
        bool isInsideBand(
            size_type row,
            size_type column) const noexcept
        {
            return column >= row
                       ? column - row <= upperBandwidth_
                       : row - column <= lowerBandwidth_;
        }

        void checkBounds(size_type row, size_type column) const
        {
            if (row >= size_ || column >= size_)
            {
                throw std::out_of_range("Band matrix index out of range.");
            }
        }

        [[nodiscard]]
        size_type index(
            size_type row,
            size_type column) const
        {
            checkBounds(row, column);
            if (!isInsideBand(row, column))
            {
                throw std::out_of_range("Band matrix index is outside the band.");
            }

            const auto diagonalIndex = column >= row
                                           ? upperBandwidth_ + column - row
                                           : upperBandwidth_ - (row - column);

            return row * (lowerBandwidth_ + upperBandwidth_ + 1)
                + diagonalIndex;
        }

        size_type size_;
        size_type lowerBandwidth_;
        size_type upperBandwidth_;

        std::vector<T> data_;

        static const T zero_;
    };

    template <typename T>
    const T BandMatrix<T>::zero_{};

    vector::Vector operator*(const BandMatrix<double>&, const vector::Vector&);
}
