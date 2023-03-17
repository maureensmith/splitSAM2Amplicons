#include "line_pair.hpp"

namespace lines
{

auto LinePair::category() const -> Category
{
    auto const cat = m_a.category();
    if (cat == m_b.category())
    {
        return cat;
    }
    return Category::FILTERED;
}

} // namespace lines

