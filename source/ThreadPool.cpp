#include "ThreadPool.h"

#include <thread>
#include "CollatzSolver.h"

#include <iostream>

ThreadPool::ThreadPool(uint64_t threads_count, std::unique_ptr<ElementsHolder> elements_holder) noexcept
    : threads_count_(threads_count), elements_holder_(std::move(elements_holder))
{
}

void ThreadPool::Execute() noexcept
{
    std::vector<std::thread> threads;
    {
        std::lock_guard<std::mutex> guard(mutex_);
        for (uint64_t i = 0; i < threads_count_ && elements_holder_->HasNext(); ++i)
        {
            threads.emplace_back(&ThreadPool::_StartThread, this, elements_holder_->Next());
        }
    }

    for (auto &thread : threads)
        if (thread.joinable())
            thread.join();
}

void ThreadPool::_StartThread(const std::vector<uint64_t> &value) noexcept
{
    std::shared_ptr<DatabaseInstance> db = db_.GetInstance();
    while (true)
    {
        std::vector<uint64_t> sqs_sequence;
        {
            std::lock_guard<std::mutex> guard(mutex_);
            if (!elements_holder_->HasNext())
            {
                break;
            }
            sqs_sequence = std::move(elements_holder_->Next());
        }
        CollatzSolver solver(std::move(sqs_sequence));
        auto solution = std::move(solver.Solve());
        std::cout << solution << "\n";
        // db->Insert(std::move(solver.Solve()));
    }
}