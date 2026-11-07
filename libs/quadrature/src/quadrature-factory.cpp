#include "quadrature-factory.hpp"
#include "left-rectangle-quadrature.hpp"
#include "right-rectangle-quadrature.hpp"
#include "midpoint-rectangle-quadrature.hpp"
#include "trapezoid-quadrature.hpp"
#include "simpson-quadrature.hpp"
#include "gauss-legendre-quadrature.hpp"
#include <stdexcept>

namespace numeric::quadratures {
    std::unique_ptr<IQuadrature> QuadratureFactory::create(QuadratureType type) {
        switch (type) {
            case QuadratureType::LeftRectangle:
                return std::make_unique<LeftRectangleQuadrature>();
            case QuadratureType::RightRectangle:
                return std::make_unique<RightRectangleQuadrature>();
            case QuadratureType::MidpointRectangle:
                return std::make_unique<MidpointRectangleQuadrature>();
            case QuadratureType::Trapezoid:
                return std::make_unique<TrapezoidQuadrature>();
            case QuadratureType::Simpson:
                return std::make_unique<SimpsonQuadrature>();
            case QuadratureType::GaussLegendre:
                return std::make_unique<GaussLegendreQuadrature>();
        }
        throw std::invalid_argument("Unknown quadrature type");
    }
} // namespace numeric::quadratures
