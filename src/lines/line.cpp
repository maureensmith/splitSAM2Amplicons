#include "line.hpp"
#include "../errors/error.hpp"

#include <algorithm>
#include <charconv>
#include <tuple>


namespace lines
{

namespace
{

auto next_tab(std::string_view line) -> std::string_view::size_type
{
    auto const pos = line.find('\t');
    if (pos == std::string_view::npos)
    {
        throw errors::ErrorId::LINE_FORMAT_BROKEN;
    }
    return pos;
}

} // namespace

unsigned long Line::ampli1_start = 19;
unsigned long Line::ampli2_start = 227;
unsigned long Line::ampli2_end = 437;


auto Line::name() const -> std::string_view
{
    auto const pos = next_tab(m_line);
    auto ret = std::string_view{m_line};
    return ret.substr(0, pos);
}


auto Line::category() const -> Category
{
    auto view = std::string_view{m_line};
    for (auto i = 0; i < 3; ++i)
    {
        auto const pos = next_tab(view);
        view.remove_prefix(pos + 1);
    }

    auto const part = view.substr(0, next_tab(view));
    auto val = 0ul;
    auto const result = std::from_chars(part.begin(), part.end(), val);
    if ((result.ptr not_eq part.end()) or (result.ec == std::errc::result_out_of_range))
    {
        throw errors::ErrorId::POS_NO_UL;
    }


    //auto constexpr AMPLI1_START = 19ul;
    //auto constexpr AMPLI2_START = 227ul;
    auto ampli1_end_exlusive = Line::ampli2_start - 1;

    if (val == std::clamp(val, Line::ampli1_start, ampli1_end_exlusive))
    {
        return Category::AMPLI_1;
    }

    //auto constexpr AMPLI2_END_EXCLUSIVE = 437ul - 1; // -1 to get clamp to behave as L <= x < M 
    auto ampli2_end_exlusive = Line::ampli2_end - 1; // -1 to get clamp to behave as L <= x < M 
    if (val == std::clamp(val, Line::ampli2_start, ampli2_end_exlusive))
    {
        return Category::AMPLI_2;
    }

    return Category::FILTERED;
}

} // namespace lines

