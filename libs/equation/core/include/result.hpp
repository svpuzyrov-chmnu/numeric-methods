#pragma once

#include <iostream>

namespace equation::core
{
    enum ResultType: int
    {
        DEFINED,
        UNDEFINED
    };

    class Result
    {
        ResultType resultType_;
        double value_;
    public:
        Result(const ResultType& result_type, const double& value)
            : resultType_(result_type),
              value_(value)
        {}
        const ResultType& resultType() const noexcept { return resultType_; }

        const double& value() const noexcept { return value_; }
    };

    std::ostream& operator<<(std::ostream& os, const Result& range);
}
