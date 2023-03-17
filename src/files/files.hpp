#pragma once

#include "input.hpp"
#include "output.hpp"
#include <fstream>
#include <string>
#include <tuple>


namespace files {
   
class Files final {
  public:
    /*!
     * \brief Initialises input and output files.
     *
     * Given a path to the input file these steps are performed:
     * - check that the file can be opened for reading
     * - create in the input's parent folder the output folders
     *   (SAM_ampli1, SAM_ampli2, SAM_filtered)
     * - initialise the output and check that they can be opened for writing.
     *   (if the files exist they will be overwritten)
     *
     * \param path the path to the input file
     *
     * \throws errors::ErrorId::CANNOT_READ if the input is not readable.
     * \throws whatever Output's ctor throws.
     */
    static auto setup(std::string_view path) -> Files;

    /// Filter the lines according to their position.
    auto filter() -> void;

    /// Get the lines per category (ampli_1, ampli_2, filtered).
    auto status() const noexcept -> std::tuple<unsigned, unsigned, unsigned>
    {
        return {m_ampli_1.count(), m_ampli_2.count(), m_filtered.count()};
    }

  private:
    // private ctor, init only possible via setup
    Files(std::ifstream&& input,
          std::string&& ampli_1,
          std::string&& ampli_2,
          std::string&& filtered);

    // the input file
    Input m_input;

    // the three output files for the categories
    Output m_ampli_1;
    Output m_ampli_2;
    Output m_filtered;
};

} // namespace files

