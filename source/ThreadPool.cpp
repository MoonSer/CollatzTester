#include "ThreadPool.h"

#include <thread>
#include "CollatzSolver.h"

ThreadPool::ThreadPool(uint64_t threads_count, std::shared_ptr<ElementsHolder> elements_holder) noexcept
    : threads_count_(threads_count), elements_holder_(elements_holder)
{
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

void ThreadPool::_StartThread(std::vector<uint64_t> value) noexcept
{
    std::shared_ptr<DatabaseInstance> db = db_.GetInstance();
    do
    {
        CollatzSolver solver(std::move(value));
        auto solution = std::move(solver.Solve());
        // {
        //     std::lock_guard<std::mutex> guard(mutex_);
        //     f_ << solution << "\n";
        // }
        {
            std::lock_guard<std::mutex> guard(mutex_);
            if (!elements_holder_->HasNext())
            {
                break;
            }
            value = std::move(elements_holder_->Next());
        }
        // db->Insert(std::move(solver.Solve()));
    } while (true);
}