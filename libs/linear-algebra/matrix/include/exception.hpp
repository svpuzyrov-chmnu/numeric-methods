#pragma once
#include <exception>

namespace linear_algebra::exception {
    struct xInvalidIndex: std::exception
    {
    private:
        const size_t index_;
    public:
        explicit xInvalidIndex(const size_t& index) : index_(index) {}
    };

    struct xOutOfRange: std::exception
    {
    private:
        const size_t row_;
        const size_t col_;
    public:
        explicit xOutOfRange(const size_t& row, const size_t& col) : row_(row), col_(col) {}
    };
}
