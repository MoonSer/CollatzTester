#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <vector>
#include <mutex>
#include <functional>

#include "ElementsHolder.h"
#include "Database.h"

template <typename ReturnType, typename ArgType>
class ThreadPool
{
public:
    ThreadPool(uint64_t threads_count, ElementsHolder<ArgType> elements_holder) noexcept;

    void Execute(std::function<ReturnType(const ArgType &)> func) noexcept;

private:
    void _StartThread(std::function<ReturnType(const ArgType &)> func) noexcept;
    void _SaveResult(mongocxx::client &client, const ArgType &value, const ReturnType &return_value);

private:
    Database db_;
    std::mutex mutex_;
    uint64_t threads_count_;
    ElementsHolder<ArgType> elements_holder_;
};

#endif // THREAD_POOL_H