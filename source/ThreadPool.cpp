#include "ThreadPool.h"

#include "CollatzSolver.h"
#include "FutureHolder.h"

ThreadPool::ThreadPool(uint64_t threads_count, std::shared_ptr<ElementsHolder> elements_holder)
    : threads_count_(threads_count), elements_holder_(elements_holder)
{
    auto [is_done, errror] = db_.SetUp();
    if (!is_done)
    {
        throw std::runtime_error(errror);
    }
}

void ThreadPool::Execute() noexcept
{
    std::vector<std::thread> threads;
    for (uint64_t i = 0; i < threads_count_; ++i)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if (!elements_holder_->HasNext())
        {
            break;
        }
        threads.emplace_back(&ThreadPool::_StartThread, this, elements_holder_->Next());
    }

    for (auto &thread : threads)
        if (thread.joinable())
            thread.join();
}

void ThreadPool::_StartThread(std::deque<uint64_t> value) noexcept
{
    std::unique_ptr<DatabaseInstance> db = db_.GetInstance();
    do
    {
#ifdef DEBUG
        auto now = std::chrono::system_clock::now();
#endif

        CollatzSolver solver(std::move(value));

#ifdef DEBUG
        auto count = (std::chrono::system_clock::now() - now);
        {
            std::lock_guard<std::mutex> guard(mutex_);
            times_.emplace_back(TimeType::Solution, count);
        }
        now = std::chrono::system_clock::now();
#endif

        db->Insert(std::move(solver.Solve()));
#ifdef DEBUG
        count = (std::chrono::system_clock::now() - now);
        {
            std::lock_guard<std::mutex> guard(mutex_);
            times_.emplace_back(TimeType::Insertion, count);
        }
        now = std::chrono::system_clock::now();
#endif

        {
            std::lock_guard<std::mutex> guard(mutex_);
            if (!elements_holder_->HasNext())
            {
                break;
            }
            value = std::move(elements_holder_->Next());
        }

#ifdef DEBUG
        count = (std::chrono::system_clock::now() - now);
        {
            std::lock_guard<std::mutex> guard(mutex_);
            times_.emplace_back(TimeType::NextIter, count);
        }
#endif
        sleep(0);

    } while (true);
    FutureHolder::instance().WaitUntillComplete();
}