#pragma once
#include "quadrature.hpp"

namespace numeric::quadratures {
    class SimpsonQuadrature final : public IQuadrature {
    public:
        [[nodiscard]] QuadratureResult integrate(const Function &,
                                                 Interval,
                                                 const QuadratureOptions &) const override;

        [[nodiscard]] QuadratureType type() const noexcept override { return QuadratureType::Simpson; }
    };
} // namespace numeric::quadratures
