#pragma once

#include "../lines/line.hpp"
#include "../lines/line_pair.hpp"
#include <fstream>
#include <optional>
#include <string>
#include <utility>


namespace files {

/// input file handler
class Input final {
  public:
    /*!
     * \brief Create a new input file handler.
     *
     * \param src An ifstream to read from.
     *
     * \note Expected: *src.good() == true*
     */
    explicit Input(std::ifstream&& src)
        : m_input{std::move(src)}
    {}

    /*!
     * \brief Read a header line.
     *
     * A header line starts with *@*.
     * The line is only extracted, iff it is a header.
     *
     * \return An optional Line if the current line is a header.
     *
     * \throws errors::ErrorId::CANNOT_READ error while reading the file
     */
    [[nodiscard]]
    auto read_header() -> lines::MaybeLine;

    /*!
     * \brief Get the two line pairs.
     *
     * \note This moves the pair.
     *
     * \return A pair containing the two lines.
     */
    [[nodiscard]]
    auto take_line_pair() -> lines::LinePair&&
    {
        return std::move(m_lines);
    }

    /*!
     * \brief Extract the next line pair.
     *
     * \pre All header lines must be consumed using read_header().
     *
     * \throws errors::ErrorId::ORPHAN only able to read one line
     */
    [[nodiscard]]
    auto next_line_pair() -> bool;

  private:
    // the input stream
    std::ifstream m_input;

    // read a single line, on error std::nullopt.
    [[nodiscard]]
    auto get_line() -> lines::MaybeLine;

    // the current line pair
    lines::LinePair m_lines{};
};

} // namespace files

