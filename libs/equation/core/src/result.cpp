#include "result.hpp"

namespace equation::core
{
    std::ostream& operator<<(std::ostream& os, const Result& result)
    {
        os << "{type: ";

        switch (result.resultType())
        {
        case DEFINED:
            os << "DEFINED";
            break;
        default:
            os << "UNDEFINED";
            break;
        }
        os << "; value: " << result.value() << "}";
        return os;
    }
}
