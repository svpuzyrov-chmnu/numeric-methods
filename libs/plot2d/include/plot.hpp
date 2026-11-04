#pragma once

#include <vector>
#include <string>
#include "curve.hpp"

namespace plot2d
{
    class Plot {
    public:
        explicit Plot(std::string title = {})
            : title_(std::move(title)) {}

        Plot& add_curve(Curve curve) {
            curves_.push_back(std::move(curve));
            return *this;
        }

        Plot& set_labels(std::string x, std::string y) {
            xLabel_ = std::move(x);
            yLabel_ = std::move(y);
            return *this;
        }

        Plot& set_grid_enabled(const bool enabled) {
            grid_enabled_ = enabled;
            return *this;
        }

        [[nodiscard]] const std::string& title() const {
            return title_;
        }

        [[nodiscard]] const std::string& xLabel() const {
            return xLabel_;
        }

        [[nodiscard]] const std::string& yLabel() const {
            return yLabel_;
        }

        [[nodiscard]] const std::vector<Curve>& curves() const {
            return curves_;
        }

        [[nodiscard]] bool grid_enabled() const {
            return grid_enabled_;
        }

    private:
        std::string title_;
        std::string xLabel_ = "x";
        std::string yLabel_ = "y";

        std::vector<Curve> curves_;
        bool grid_enabled_ = true;
    };

}
