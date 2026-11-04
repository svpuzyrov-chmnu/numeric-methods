#include "app.hpp"
#include <iostream>
#include "mean-square.hpp"
#include "plotter.hpp"

void process_mean_square() {
    using std::cout;
    using std::cin;
    using std::endl;

    using plane2d::Points;
    using plane2d::tabulate;
    using interpolation::mean_square::MeanSquare;
    using plot2d::Plot;
    using plot2d::Plotter;

    //v25
    Points points{
        {1.231, 0.839}, {1.684, 1.498}, {1.921, 1.781}, {2.136, 2.023}, {2.174, 2.066}, {2.867, 2.805}, {4.873, 4.852},
        {6.312, 6.3}
    };

    cout << "Interpolation points: " << points << endl;

    const MeanSquare mean_square(points);

    auto line_approx = mean_square.approx(1);

    auto quadratic_approx = mean_square.approx(2);

    auto cubic_approx = mean_square.approx(3);

    const auto left_p = plane2d::left_point(points);

    const auto right_p = plane2d::right_point(points);

    const auto liner_approx_points = tabulate([&line_approx](const double x) { return line_approx(x); },
                                              left_p.x - 1,
                                              right_p.x + 1, 50);

    const auto quadratic_approx_points = tabulate([&quadratic_approx](const double x) { return quadratic_approx(x); },
                                                  left_p.x - 1,
                                                  right_p.x + 1, 50);

    const auto cubic_approx_points = tabulate([&cubic_approx](const double x) { return cubic_approx(x); },
                                              left_p.x - 1,
                                              right_p.x + 1, 50);

    Plot plot("Mean Square Approximation");

    plot.set_labels("X", "Y")
            .set_grid_enabled(true)
            .add_curve({points, "Source points", plot2d::CurveStyle::LineWithPoints, 2})
            .add_curve({liner_approx_points, "Linear approximation", plot2d::CurveStyle::Line, 2})
            .add_curve({quadratic_approx_points, "Quadratic approximation", plot2d::CurveStyle::Line, 2})
            .add_curve({cubic_approx_points, "Cubic approximation", plot2d::CurveStyle::Line, 2});

    constexpr Plotter plotter;

    plotter.show_and_wait(plot);
}
