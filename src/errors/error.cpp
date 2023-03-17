#include "error.hpp"

namespace errors
{

auto message(ErrorId const id) -> std::string_view
{
    switch (id)
    {
        case ErrorId::LINE_FORMAT_BROKEN: return "wrong line format (missing \\t)";
        case ErrorId::POS_NO_UL: return "cannot parse position";
        case ErrorId::CANNOT_WRITE: return "no write access to output file";
        case ErrorId::CANNOT_READ: return "no read access to input file";
        case ErrorId::WRONG_ORDER: return "something wrong with order";
        case ErrorId::ORPHAN: return "orphaned line";
        case ErrorId::UNKNOWN_CATEGORY: return "unknown category (this must not happen)";
        default: return "unknown error";
    }
}

} // namespace errors

