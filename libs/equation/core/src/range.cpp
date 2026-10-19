#include "range.hpp"
namespace equation::core
{
    std::ostream& operator<<(std::ostream& os, const Range& range)
    {
        os << "[" << range.left() << "; " << range.right() << "]";
        return os;
    }
}
