#include "ThreadPool.h"

template <typename ReturnType, typename ArgType>
ThreadPool<ReturnType, ArgType>::ThreadPool(uint64_t threads_count, ElementsHolder<ArgType> elements_holder) noexcept
    : threads_count_(threads_count), elements_holder_(elements_holder)
{
}

template <typename ReturnType, typename ArgType>
void ThreadPool<ReturnType, ArgType>::Execute(std::function<ReturnType(const ArgType &)> func) noexcept
{
    std::vector<std::thread> threads;
    for (uint64_t i = 0; i < threads_count_; ++i)
    {
        threads.emplace_back(&ThreadPool<ReturnType, ArgType>::_StartThread, this, func);
    }

    for (auto &thread : threads)
        if (thread.joinable())
            thread.join();
}

template <typename ReturnType, typename ArgType>
void ThreadPool<ReturnType, ArgType>::_StartThread(std::function<ReturnType(const ArgType &)> func) noexcept
{
    auto client = mongodb_pool_.acquire();
    while (elements_holder_.HasNext())
    {
        ArgType value;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            value = elements_holder_.Next();
        }

        _SaveResult(client, value, func(value));
    }
}

template <typename ReturnType, typename ArgType>
void _SaveResult(mongocxx::client &client, const ArgType &value, const ReturnType &return_value)
{
    db_.Save(client, value, return_value);
}