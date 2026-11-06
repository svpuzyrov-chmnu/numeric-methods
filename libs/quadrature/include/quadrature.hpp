#pragma once
#include "quadrature-types.hpp"

namespace numeric::quadratures {
    class IQuadrature {
    public:
        virtual ~IQuadrature() = default;

        [[nodiscard]] virtual QuadratureResult integrate(const Function &f,
                                                         Interval interval,
                                                         const QuadratureOptions &options) const = 0;

        [[nodiscard]] virtual QuadratureType type() const noexcept = 0;
    };
} // namespace numeric::quadratures
