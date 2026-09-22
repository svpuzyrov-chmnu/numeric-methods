#include <catch2/catch_test_macros.hpp>

#include "vector.hpp"

TEST_CASE("Vector construction")
{
    using linear_algebra::vector::Vector;

    SECTION("Default constructor")
    {
        Vector v;
        REQUIRE(v.size() == 0);
    }

    SECTION("Size constructor")
    {
        Vector v(5);
        REQUIRE(v.size() == 5);
        for (size_t i = 0; i < v.size(); ++i)
        {
            REQUIRE(v[i] == 0.0);
        }
    }

    SECTION("Initializer list constructor")
    {
        Vector v{1.0, 2.0, 3.0};
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 1.0);
        REQUIRE(v[1] == 2.0);
        REQUIRE(v[2] == 3.0);
    }

    SECTION("Generated constructor")
    {
        Vector v{3, [](const size_t& i) { return static_cast<double>(i + 2); }};
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 2.0);
        REQUIRE(v[1] == 3.0);
        REQUIRE(v[2] == 4.0);
    }

    SECTION("Generated constructor")
    {
        Vector v{3, [](const size_t& i) { return static_cast<double>(i + 2); }};
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 2.0);
        REQUIRE(v[1] == 3.0);
        REQUIRE(v[2] == 4.0);
    }

    SECTION("Create from raw static array")
    {
        double arr[] = {5.0, -12.0, 5.12, 7.11, 8.51};
        Vector v(arr);
        REQUIRE(v.size() == 5);
        REQUIRE(v[0] == 5.0);
        REQUIRE(v[1] == -12.0);
        REQUIRE(v[2] == 5.12);
    }
}