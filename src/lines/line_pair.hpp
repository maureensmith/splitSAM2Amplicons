#pragma once

#include "category.hpp"
#include "line.hpp"
#include <string>
#include <utility>


namespace lines
{

/// A pair of two read lines.
class LinePair final
{
  public:
    /// default ctor
    LinePair() = default;

    /// Initialise a new line pair from given lines.
    LinePair(Line a, Line b)
        : m_a{std::move(a)},
          m_b{std::move(b)}
    {
    }

    /*!
     * \brief Checks that the lines have the same name.
     *
     * \throws \see Line::name()
     */
    [[nodiscard]]
    auto same_names() const -> bool
    {
        return m_a.name() == m_b.name();
    }
    
    /*!
     * \brief Get the category of the pair.
     *
     * If the lines have the same category, returns this one.
     * Otherwise Category::FILTERED is returned.
     *
     * \throws \see Line::category()
     */
    [[nodiscard]]
    auto category() const -> Category;


    /// move the first line's content
    [[nodiscard]]
    auto take_first_string() -> std::string&&
    {
        return std::move(m_a.take());
    }
   
    /// move the second line's content
    [[nodiscard]]
    auto take_second_string() -> std::string&&
    {
        return std::move(m_b.take());
    }

  private:
    Line m_a{};
    Line m_b{};
};

} // namespace lines

