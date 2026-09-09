#include <iostream>
#include <iomanip>
#include <cmath>
#include "functional-sequence-helper.hpp"
#include "functional-sequence-teilor.hpp"

void enter_point(const double& a, const double& b, double &x);

void enter_tolerance(double &tolerance);

bool check_continue();

double multiplier(const double& x, const int &);

double start_evaluator(const double& x);

double source_function(const double& x);

void process_compute();
