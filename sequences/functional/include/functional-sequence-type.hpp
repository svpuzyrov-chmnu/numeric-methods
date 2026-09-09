#pragma once


namespace math_helpers::sequences::functional {
    typedef double (*term_multiplier_t)(const double &, const int &);

    typedef double (*function1_t)(const double &);
}
