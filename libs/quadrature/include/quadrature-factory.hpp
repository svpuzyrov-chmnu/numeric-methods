#pragma once
#include "quadrature.hpp"
#include <memory>

namespace numeric::quadratures {
    class QuadratureFactory final {
    public:
        static std::unique_ptr<IQuadrature> create(QuadratureType type);
    };
} // namespace numeric::quadratures
