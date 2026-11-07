#pragma once
#include "quadrature.hpp"

namespace numeric::quadratures {
    class RightRectangleQuadrature final : public IQuadrature {
    public:
        [[nodiscard]] QuadratureResult integrate(const Function &,
                                                 Interval,
                                                 const QuadratureOptions &) const override;

        [[nodiscard]] QuadratureType type() const noexcept override { return QuadratureType::RightRectangle; }
    };
} // namespace numeric::quadratures
