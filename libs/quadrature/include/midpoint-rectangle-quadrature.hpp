#pragma once
#include "quadrature.hpp"

namespace numeric::quadratures {
    class MidpointRectangleQuadrature final : public IQuadrature {
    public:
        [[nodiscard]] QuadratureResult integrate(const Function &,
                                                 Interval,
                                                 const QuadratureOptions &) const override;

        [[nodiscard]] QuadratureType type() const noexcept override {
            return QuadratureType::MidpointRectangle;
        }
    };
} // namespace numeric::quadratures
