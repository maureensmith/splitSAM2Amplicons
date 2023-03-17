#pragma once

#include <condition_variable>
#include <deque>
#include <fstream>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>


namespace files {

/*!
 * \brief Output file handler.
 *
 * Data send to this file handler are stored in a queue.
 * After enough data is collected an internal thread writes to the file.
 * All remaining data is written on destruction.
 */
class Output final {
  public:
    /*!
     * \brief Create a new output file handler.
     *
     * \param path The path of the file.
     *
     * \throws ErrorId::NO_WRITE file cannot be opened for writing.
     *
     * \see files::Files::setup
     */
    explicit Output(std::string&& path);

    /*!
     * \brief Add lines to the queue.
     *
     * If BUCKET_SIZE pairs have been add they are written to the file by the internal thread.
     *
     * \tparam Count increase the bucket counter, default is true.
     * \tparam S type of the input (should be anything that can be *std::move*d to *std::string*.
     * \param lines the lines to add
     *
     * \note No checks on S are performed.
     */
    template <bool Count = true,
             typename... S>
    auto write(S... lines) -> void
    {
        if constexpr (Count)
        {
            ++m_counter;
        }
        
        auto lock = std::unique_lock{m_mtx};
        (m_data.emplace_back(std::move(lines)), ...);

        if (m_data.size() > BUCKET_SIZE)
        {
            lock.unlock();
            m_cv.notify_one();
        }
    }

    /*!
     * \brief Write a header line.
     *
     * Same as write, but does not increase the counter.
     * This is syntactic sugar for *write<false>(line);*.
     *
     * \tparam T same as write's S
     * \param line the line to add
     */
    template <typename T>
    auto write_header_line(T line) -> void
    {
        write<false>(std::move(line));
    }

    /// Number of pairs written.
    [[nodiscard]]
    auto count() const noexcept -> unsigned { return m_counter; }

    /// Write remaining data and wait for the thread to finish.
    ~Output();

  private:
    // the path of the file
    std::string const m_path;

    // number of pairs written
    unsigned m_counter{0};

    // data queue
    std::deque<std::string> m_data{};

    // protection for the data queue
    std::mutex m_mtx{};

    // notification for stop request and data writing
    std::condition_variable m_cv{};

    // the thread writing to file
    std::jthread m_thread{};

    // worker function of the thread
    auto write(std::stop_token stoken) -> void;

    // write the data
    auto to_file(std::deque<std::string> const& local) -> void;

    // number of elements for a batch of writing
    static unsigned constexpr BUCKET_SIZE = 50'000;
};

} // namespace files

