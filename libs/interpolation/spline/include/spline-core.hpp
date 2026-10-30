#pragma once

#include <stdexcept>
#include <vector>
#include <array>

#include "interpolation-core.hpp"

namespace interpolation::spline::core {
    using interpolation::core::Point;
    using interpolation::core::Points;

    class Node {
    public:
        Node(const int &index, const Point &point)
            : index_(index),
              point_(point) {
        }

        [[nodiscard]] int index() const {
            return index_;
        }

        [[nodiscard]] Point point() const {
            return point_;
        }

    private:
        int index_;
        Point point_;
    };

    using Nodes = std::vector<Node>;

    template<int degree>
    class Segment {
    public:
        using Coefficients = std::array<double, degree + 1>;

        explicit Segment(const Coefficients &coefficients,
                         const Node &start,
                         const Node &end)
            : coefficients_(coefficients),
              start_(start),
              end_(end) {
        }

        [[nodiscard]] Node start() const {
            return start_;
        }

        [[nodiscard]] Node end() const {
            return end_;
        }

        [[nodiscard]] double get_coefficient(int index) const {
            check_index(index);
            return coefficients_[index];
        }

    private:
        static void check_index(int index) {
            if (index < 0 || index > degree) {
                throw std::out_of_range(
                    "Invalid index of spline segment coefficient"
                );
            }
        }

        //a=weights[0], b=weights[1], c=weights[2], d=weights[3] etc
        Coefficients coefficients_;
        Node start_;
        Node end_;
    };

    Nodes create_from(const Points &);
}
