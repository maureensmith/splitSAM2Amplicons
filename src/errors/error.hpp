#pragma once

#include <string_view>

// contains all possible errors
namespace errors
{

/// Possible error Ids
enum class ErrorId
{
    /// Line has the wrong format (not enough \t)
    LINE_FORMAT_BROKEN = 1,

    /// Position cannot be converted to unsigned long
    POS_NO_UL,

    /// No permissions to write
    CANNOT_WRITE,

    /// No permissions to write
    CANNOT_READ,

    /// Order of input file broken
    WRONG_ORDER,

    /// Unable to read a line pair
    ORPHAN,

    /// Line has unknown categors (impossible!)
    UNKNOWN_CATEGORY,

    /// this error is not covered
    NOT_COVERED
};

/// Convert the Id to a message.
[[nodiscard]]
auto message(ErrorId id) -> std::string_view;

} // namespace errors

