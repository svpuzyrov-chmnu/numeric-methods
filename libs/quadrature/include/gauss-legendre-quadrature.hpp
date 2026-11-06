#pragma once
#include "quadrature.hpp"

namespace numeric::quadratures {
    class GaussLegendreQuadrature final : public IQuadrature {
    public:
        [[nodiscard]] QuadratureResult integrate(const Function &,
                                                 Interval,
                                                 const QuadratureOptions &) const override;

        [[nodiscard]] QuadratureType type() const noexcept override {
            return QuadratureType::GaussLegendre;
        }

        [[nodiscard]] static std::vector<QuadratureNode> canonicalNodes(std::size_t order);
    };
} // namespace numeric::quadratures
