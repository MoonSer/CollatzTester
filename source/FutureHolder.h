#pragma once
#ifndef FUTURE_HOLDER_H
#define FUTURE_HOLDER_H

#include <cassandra.h>
#include <set>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <functional>

class FutureHolder
{
public:
    static FutureHolder &instance() noexcept
    {
        static FutureHolder holder;
        return holder;
    }

    static void FutureDone(CassFuture *future, void *data) noexcept
    {
        static_cast<FutureHolder *>(data)->FutureDone_(future);
    }

    void FutureDone_(CassFuture *future) noexcept
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            futures_.erase(future);
            // std::cout << "Callback done. poll: " << to_execute_.size() << "  futures: " << futures_.size() << "\n";
        }

        if (cass_future_error_code(future) != CASS_OK)
        {
            const char *message;
            size_t message_length;
            cass_future_error_message(future, &message, &message_length);

            std::lock_guard<std::mutex> lock(mutex_);
            errors_.emplace_back(message);
        }
        cass_future_free(future);

        if (!ExecuteFutureInPoll())
        {
            cv_.notify_all();
        }
    }

    std::vector<std::string> GetErrors()
    {
        return errors_;
    }

    bool ExecuteFutureInPoll() noexcept
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (futures_.size() < max_count_ && to_execute_.size() > 0)
        {
            CassFuture *future = to_execute_.back()();
            to_execute_.pop_back();
            cass_future_set_callback(future, &FutureHolder::FutureDone, this);
            futures_.insert(future);
            return true;
        }
        return false;
    }

    void PushWork(std::function<CassFuture *(void)> &&executor) noexcept
    {
        {
            std::lock_guard<std::mutex> guard(mutex_);
            to_execute_.push_back(std::move(executor));
        }
        if (futures_.size() < max_count_)
        {
            ExecuteFutureInPoll();
        }
    }

    void WaitUntillComplete() noexcept
    {
        std::unique_lock<std::mutex> lock(all_done_mutex_);
        while (futures_.size() > 0 || to_execute_.size() > 0)
        {
            cv_.wait(lock);
        }
    }

private:
    FutureHolder(uint32_t max_count = 8192)
        : max_count_(max_count)
    {
    }

private:
    std::set<CassFuture *> futures_;
    uint32_t max_count_;

    std::mutex mutex_;
    std::mutex all_done_mutex_;
    std::condition_variable cv_;
    std::vector<std::string> errors_;
    std::vector<std::function<CassFuture *(void)>> to_execute_;
};
#endif // FUTURE_HOLDER_H