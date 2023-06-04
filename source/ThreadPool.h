#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <mutex>
#include <memory>

#include "ElementsHolder.h"
#include "Database.h"

#ifdef DEBUG
#include <chrono>
enum TimeType
{
    Solution,
    Insertion,
    NextIter
};
#endif

class ThreadPool
{
public:
    ThreadPool(uint64_t threads_count, std::shared_ptr<ElementsHolder> elements_holder);

    void Execute() noexcept;

#ifdef DEBUG
    std::vector<std::pair<TimeType, std::chrono::nanoseconds>> GetTimes() const noexcept
    {
        return times_;
    }
#endif

private:
    void _StartThread(std::deque<uint64_t> value) noexcept;

private:
    std::mutex mutex_;
    Database db_;
    uint64_t threads_count_;
    std::shared_ptr<ElementsHolder> elements_holder_;

#ifdef DEBUG
    std::vector<std::pair<TimeType, std::chrono::nanoseconds>> times_;
#endif
};

#endif // THREAD_POOL_H