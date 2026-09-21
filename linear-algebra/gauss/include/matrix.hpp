#pragma once
#include <vector>
#include "vector.hpp"

namespace linear_algebra::matrix {
    class Matrix final {
        std::vector<vector::Vector> rows_;
    public:
        Matrix() = default;

        Matrix(const size_t rows, const size_t cols)
            :rows_(rows)
        {
            for (size_t i = 0; i < rows_.size(); ++i)
            {
                rows_.emplace_back(cols);
            }
        }

        double operator()(const size_t& i, const size_t& j) const { return rows_.at(i).at(j); }

        double& operator()(const size_t& i, const size_t& j)
        {
            auto& row = this->rows_.at(i);

            return row[j];
        }
    };
}
