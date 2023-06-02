#include "ElementsHolder.h"

ElementsHolder::ElementsHolder(std::vector<uint64_t> &&from, uint64_t max_item, uint64_t max_size) noexcept
    : current_(std::move(from)), max_item_(max_item), max_size_(max_size)
{
}

std::vector<uint64_t> ElementsHolder::Next() noexcept
{
    current_.back() += 1;

    for (uint64_t i = current_.size() - 1; i > 0 && current_[i] == max_item_; --i)
    {
        current_[i] = 1;
        current_[i - 1] += 1;
    }
    if (current_.front() == max_item_)
    {
        current_.front() = 1;
        current_.insert(current_.begin(), {1, 1});
    }

    return current_;
}

bool ElementsHolder::HasNext() const noexcept
{
    if (current_.size() > max_size_)
        return false;

    for (const auto &value : current_)
    {
        if (value < max_item_ - 1 || current_.size() < max_size_)
        {
            return true;
        }
    }
    return false;
}
