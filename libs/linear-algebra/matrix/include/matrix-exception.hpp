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

    struct xImpossibleMatrixOperation : std::exception
    {
    };

    struct xNonEqualMatrixDimensions : xImpossibleMatrixOperation
    {
    private:
        const size_t rows_;
        const size_t cols_;

    public:
        xNonEqualMatrixDimensions(const size_t rows, const size_t cols)
            : rows_(rows)
        , cols_(cols)
        {
        }

        [[nodiscard]] const char* what() const noexcept override
        {
            return "Matrix dimensions are not equal.";
        }
    };
}
