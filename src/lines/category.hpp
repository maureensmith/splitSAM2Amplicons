#pragma once

namespace lines
{

/// Possible categories of a line
enum class Category
{
    /// Line's pos is AMPLI_1
    AMPLI_1,
    /// Line's pos is AMPLI_2
    AMPLI_2,
    /// Line's pos is FILTERED (= anything else)
    FILTERED
};

} // namespace lines

