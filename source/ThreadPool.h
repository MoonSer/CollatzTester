#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <mutex>
#include <memory>

#include "ElementsHolder.h"
#include "Database.h"

class ThreadPool
{
public:
    ThreadPool(uint64_t threads_count, std::unique_ptr<ElementsHolder> elements_holder) noexcept;

    void Execute() noexcept;

private:
    void _StartThread(const std::vector<uint64_t> &value) noexcept;

private:
    Database db_;
    std::mutex mutex_;
    uint64_t threads_count_;
    std::unique_ptr<ElementsHolder> elements_holder_;
};

#endif // THREAD_POOL_H