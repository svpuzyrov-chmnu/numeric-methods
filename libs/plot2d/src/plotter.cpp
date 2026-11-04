#include "plotter.hpp"

#include <sciplot/sciplot.hpp>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace plot2d {
    namespace {
        void validate(const Plot &plot) {
            if (plot.curves().empty()) {
                throw std::invalid_argument(
                    "Plot must contain at least one curve"
                );
            }

            for (const auto &curve: plot.curves()) {
                if (curve.points.empty()) {
                    throw std::invalid_argument(
                        "Curve must contain at least one point"
                    );
                }

                if (curve.lineWidth <= 0) {
                    throw std::invalid_argument(
                        "Line width must be positive"
                    );
                }

                for (const auto &point: curve.points) {
                    if (!std::isfinite(point.x) ||
                        !std::isfinite(point.y)) {
                        throw std::invalid_argument(
                            "Plot coordinates must be finite"
                        );
                    }
                }
            }
        }

        sciplot::Plot2D makePlot(const Plot &source) {
            validate(source);

            sciplot::Plot2D result;

            result.xlabel(source.xLabel());
            result.ylabel(source.yLabel());

            if (source.grid_enabled()) {
                result.grid().show();
            }

            for (const auto &curve: source.curves()) {
                std::vector<double> x;
                std::vector<double> y;

                x.reserve(curve.points.size());
                y.reserve(curve.points.size());

                for (const auto &point: curve.points) {
                    x.push_back(point.x);
                    y.push_back(point.y);
                }

                switch (curve.style) {
                    case CurveStyle::Line:
                        result.drawCurve(x, y)
                                .label(curve.label)
                                .lineWidth(curve.lineWidth);
                        break;

                    case CurveStyle::Points:
                        result.drawPoints(x, y)
                                .label(curve.label);
                        break;

                    case CurveStyle::LineWithPoints:
                        result.drawCurveWithPoints(x, y)
                                .label(curve.label)
                                .lineWidth(curve.lineWidth);
                        break;
                }
            }

            return result;
        }

        sciplot::Canvas makeCanvas(const Plot &plot) {
            auto graph = makePlot(plot);

            sciplot::Figure figure = {{graph}};

            if (!plot.title().empty()) {
                figure.title(plot.title());
            }

            sciplot::Canvas canvas = {{figure}};
            canvas.size(1000, 650);

            return canvas;
        }
    } // anonymous namespace

    void Plotter::show(const Plot &plot) const {
        const auto canvas = makeCanvas(plot);
        canvas.show();
    }

    void Plotter::show_and_wait(const Plot &plot) const {
        show(plot);
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cin.get();
    }

    void Plotter::save(
        const Plot &plot,
        const std::string &filename
    ) const {
        if (filename.empty()) {
            throw std::invalid_argument(
                "Output filename must not be empty"
            );
        }

        const auto canvas = makeCanvas(plot);
        canvas.save(filename);
    }
}
