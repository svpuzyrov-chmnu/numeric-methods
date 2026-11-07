#pragma once
#include "quadrature.hpp"

namespace numeric::quadratures {
    class LeftRectangleQuadrature final : public IQuadrature {
    public:
        [[nodiscard]] QuadratureResult integrate(const Function &,
                                                 Interval,
                                                 const QuadratureOptions &) const override;

        [[nodiscard]] QuadratureType type() const noexcept override {
            return QuadratureType::LeftRectangle;
        }
    };
} // namespace numeric::quadratures
