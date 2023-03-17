#include "output.hpp"
#include "../errors/error.hpp"
#include <algorithm>
#include <iterator>
#include <utility>


namespace files
{

Output::Output(std::string&& path)
    : m_path{std::move(path)},
      m_thread{[this](std::stop_token stoken) { this->write(std::move(stoken)); }} 
      // this should be ok: only accesses data that is initialised before
{
    auto test = std::ofstream{m_path};
    if (not test.good())
    {
        throw errors::ErrorId::CANNOT_WRITE;
    }
}
    
Output::~Output()
{
    m_thread.request_stop();
    m_cv.notify_one();
}


auto Output::write(std::stop_token stoken) -> void
{
    while (not stoken.stop_requested())
    {
        auto lock = std::unique_lock{m_mtx};
        // if stop is requested: write the remaining data
        // if no stop requested: write the batch
        m_cv.wait(lock, [this, &stoken]
        { 
            return stoken.stop_requested() 
            || (m_data.size() > BUCKET_SIZE);
        });

        auto const local = std::exchange(m_data, {});
        lock.unlock();
        to_file(local);
    }

    // no need for protection, no data will be pushed anymore
    to_file(m_data);
}


auto Output::to_file(std::deque<std::string> const& local) -> void
{
    // opening the file and closing seems to be faster than 
    // writing everything and syncing huge amount of data
    auto file = std::ofstream{m_path, std::ios::app};
    auto it = std::ostream_iterator<std::string>{file, "\n"};
    std::copy(local.begin(), local.end(), it);
}

} // namespace files

