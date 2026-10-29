#include "least-squares.hpp"
#include "matrix.hpp"
#include "gauss.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace interpolation::list_squares {
    using linear_algebra::matrix::simple::Matrix;

    using linear_algebra::matrix::simple::Vector;

    using linear_algebra::gauss::simple::resolve;



    LeastSquares::LeastSquares(core::Points &points)
        : points_(std::move(points))
    {
        validate(points_);
    }

    polynomial::Polynomial LeastSquares::approx(const std::size_t &degree) const {
        if (const std::size_t n = points_.size(); degree >= n) {
            throw std::invalid_argument(
                "Polynomial degree must be less than point count"
            );
        }

        const std::size_t size = degree + 1;

        Matrix A(size, Vector(size, 0.0));
        Vector b(size, 0.0);

        for (const auto&[x, y] : points_) {

            Vector powers(size, 1.0);

            for (std::size_t j = 1; j < size; ++j) {
                powers[j] = powers.at(j - 1) * x;
            }

            for (std::size_t k = 0; k < size; ++k) {

                b[k] += y * powers[k];

                for (std::size_t j = 0; j < size; ++j) {
                    A[k][j] += powers[k] * powers[j];
                }
            }
        }

        Vector coefficients = resolve(A, b);

        return polynomial::Polynomial(std::move(coefficients));
    }

    double LeastSquares::squaredError(const polynomial::Polynomial &polynomial) const {
        double error = 0.0;

        for (const auto&[x, y] : points_) {
            const double difference = y - polynomial.evaluate(x);

            error += difference * difference;
        }

        return error;
    }
}
