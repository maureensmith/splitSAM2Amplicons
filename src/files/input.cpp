#include "input.hpp"
#include "../errors/error.hpp"


namespace files
{

auto Input::read_header() -> lines::MaybeLine
{
    if (not m_input.good())
    {
        throw errors::ErrorId::CANNOT_READ;
    }

    if (m_input.peek() != '@')
    {
        return std::nullopt;
    }

    return get_line();
}


auto Input::get_line() -> lines::MaybeLine
{
    auto line = std::string{};
    std::getline(m_input, line);
    if (not m_input.good())
    {
        return std::nullopt;
    }
    return {lines::Line{std::move(line)}};
}


auto Input::next_line_pair() -> bool
{
    auto line_a = get_line();
    if (not line_a)
    {
        // eof reached, all good
        return false;
    }
    
    auto line_b = get_line();
    if (not line_b)
    {
        // this line must be available
        throw errors::ErrorId::ORPHAN;
    }

    m_lines = {std::move(*line_a), std::move(*line_b)};

    return true;
}

} // namespace files

