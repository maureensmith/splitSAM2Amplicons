#include "files.hpp"
#include "../errors/error.hpp"
#include "../lines/line.hpp"

#include <filesystem>
#include <iostream>
#include <string>


namespace files
{

namespace fs = std::filesystem;

auto Files::setup(std::string_view const path) -> Files
{
    auto name = fs::path{path};

    auto input = std::ifstream{name};
    if (not input.good())
    {
        throw errors::ErrorId::CANNOT_READ;
    }

    auto const filename = name.filename();
    auto const parent = name.parent_path();

    auto ampli_1 = parent / "SAM_ampli1";
    auto ampli_2 = parent / "SAM_ampli2";
    auto filtered = parent / "SAM_filtered";

    fs::create_directory(ampli_1);
    fs::create_directory(ampli_2);
    fs::create_directory(filtered);

    return Files{
        std::move(input),
        ampli_1 /= filename, 
        ampli_2 /= filename,
        filtered /= filename
    };
}


Files::Files(std::ifstream&& input,
             std::string&& ampli_1,
             std::string&& ampli_2,
             std::string&& filtered)
    : m_input{std::move(input)},
      m_ampli_1{std::move(ampli_1)},
      m_ampli_2{std::move(ampli_2)},
      m_filtered{std::move(filtered)}
{
    while (true)
    {
        auto line = m_input.read_header();
        if (not line)
        {
            break;
        }
        m_ampli_1.write_header_line(line->borrow());
        m_ampli_2.write_header_line(line->borrow());
        m_filtered.write_header_line(line->take());
    }
}


auto Files::filter() -> void
{
    using namespace lines;
    auto constexpr OUT = 100'000ul;
    auto processed = 0ul;

    while (m_input.next_line_pair())
    {
        auto lines = m_input.take_line_pair();

        // sanity check
        if (not lines.same_names())
        {
            throw errors::ErrorId::WRONG_ORDER;
        }


        // check pos and write
        switch (lines.category())
        {
            case Category::AMPLI_1: m_ampli_1.write(lines.take_first_string(), lines.take_second_string());
                                    break;
            case Category::AMPLI_2: m_ampli_2.write(lines.take_first_string(), lines.take_second_string());
                                    break;
            case Category::FILTERED: m_filtered.write(lines.take_first_string(), lines.take_second_string());
                                     break;
            default: throw errors::ErrorId::UNKNOWN_CATEGORY;
        }

        ++processed;
        if (processed % OUT == 0)
        {
            std::cout << "Processed " << processed << " pairs" << std::endl;
        }
    }
}

} // namespace files

