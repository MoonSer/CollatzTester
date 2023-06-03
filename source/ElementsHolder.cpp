#include "ElementsHolder.h"

ElementsHolder::ElementsHolder(std::vector<uint64_t> &&from, uint64_t max_item, uint64_t max_size) noexcept
    : current_(std::move(from)), max_item_(max_item), max_size_(max_size), work_total_(0), work_done_(0)
{
    // Calculate total sum
    // Firstly, calculate current remaining count (size as current_, elements from current_ to max)
    mpz_class tmp = 1;
    for (std::size_t i = 0; i < current_.size(); ++i)
    {
        // Specified, that (current_.size() - 1) element - less by one
        if (i == current_.size() - 1)
        {
            tmp *= max_item_ - current_[i] - 1;
        }
        else
        {
            tmp *= max_item_ - current_[i];
        }
    }
    work_total_ += tmp;

    // Calculate remaing (size > current_)
    for (uint64_t i = ((current_.size() - 3) >> 1) + 1; i <= ((max_size_ - 3) >> 1); ++i)
    {
        mpz_class tmp = max_item_ - 1;
        mpz_pow_ui(tmp.get_mpz_t(), tmp.get_mpz_t(), 3 + 2 * i);
        work_total_ += tmp;
    }
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

    work_done_ += 1;

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

const mpz_class &ElementsHolder::GetTotalCount() const noexcept
{
    return work_total_;
}

const mpz_class &ElementsHolder::GetDoneCount() const noexcept
{
    return work_done_;
}
