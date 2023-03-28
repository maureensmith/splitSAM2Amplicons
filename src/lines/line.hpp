#pragma once

#include "category.hpp"
#include <optional>
#include <string>
#include <string_view>
#include <utility>


/// Contains everything needed to process a line read from an input file.
namespace lines
{

/// Handle a read line.
class Line final
{
  public:
    static unsigned long ampli1_start;
    static unsigned long ampli2_start;
    static unsigned long ampli2_end;


    Line() = default; // ctor for empty line... maybe not needed
    /*!
     * \brief Initialise a new Line from a given string.
     *
     * \param line the content.
     */
    explicit Line(std::string&& line) noexcept
        : m_line{std::move(line)}
    {
    }

    /// Borrow the line's content.
    [[nodiscard]]
    auto borrow() const noexcept -> std::string_view
    {
        return m_line;
    }

    /// Take ownership of the line's content.
    [[nodiscard]]
    auto take() noexcept -> std::string&&
    {
        return std::move(m_line);
    }

    /*!
     * \brief Get the name of the entry.
     *
     * The name is in the first column.
     *
     * \throws errors::ErrorId::LINE_FORMAT_BROKEN no first column available.
     */
    [[nodiscard]]
    auto name() const -> std::string_view;

    /*!
     * \brief Get the category of the entry.
     *
     * The category is determined by its position.
     * The position is found in the 4th column.
     *
     * \throws errors::ErrorId::LINE_FORMAT_BROKEN the position cannot be extracted
     * \throws errors::ErrorId::POS_NO_UL the position cannot be parsed to unsigned long
     */
    [[nodiscard]]
    auto category() const -> Category;

  private:
    // the string representing the line
    std::string m_line{};
};

/// Less typing in case a line cannot be read.
using MaybeLine = std::optional<Line>;

} // namespace lines

