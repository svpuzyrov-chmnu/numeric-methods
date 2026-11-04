#pragma once

#include "plot.hpp"

#include <string>

namespace plot2d {

    class Plotter {
    public:
        void show(const Plot& plot) const;

        void show_and_wait(const Plot& plot) const;

        void save(
            const Plot& plot,
            const std::string& filename
        ) const;

    };

}