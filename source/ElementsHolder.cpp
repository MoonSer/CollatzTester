#include "ElementsHolder.h"

ElementsHolder::ElementsHolder(std::deque<uint64_t> &&from, uint64_t max_item, uint64_t max_size) noexcept
    : current_sequence_(std::move(from)), max_item_(max_item), max_sequence_size_(max_size), work_total_(0), work_done_(0)
{
    // Calculate total sum
    // Firstly, calculate current remaining count (size as current_sequence_, elements from current_sequence_ to max)
    mpz_class tmp = 0;
    for (std::size_t i = 0; i < current_sequence_.size(); ++i)
    {
        // Specified, that (current_sequence_.size() - 1) element - less by one
        tmp *= (max_item_ - 1);
        tmp += max_item_ - current_sequence_[i] - 1;
    }
    work_total_ += tmp;

    // Calculate remaing (size > current_sequence_)
    for (uint64_t i = ((current_sequence_.size() - 3) >> 1) + 1; i <= ((max_sequence_size_ - 3) >> 1); ++i)
    {
        mpz_class tmp2 = max_item_ - 1;
        mpz_pow_ui(tmp2.get_mpz_t(), tmp2.get_mpz_t(), 3 + 2 * i);
        work_total_ += tmp2;
    }
}

std::deque<uint64_t> ElementsHolder::Next() noexcept
{
    current_sequence_.back() += 1;

    for (uint64_t i = current_sequence_.size() - 1; i > 0 && current_sequence_[i] == max_item_; --i)
    {
        current_sequence_[i] = 1;
        current_sequence_[i - 1] += 1;
    }
    if (current_sequence_.front() == max_item_)
    {
        current_sequence_.front() = 1;
        current_sequence_.push_front(1);
        current_sequence_.push_front(1);
    }

    work_done_ += 1;

    return current_sequence_;
}

bool ElementsHolder::HasNext() const noexcept
{
    if (current_sequence_.size() > max_sequence_size_)
        return false;

    for (const auto &value : current_sequence_)
    {
        if (value < max_item_ - 1 || current_sequence_.size() < max_sequence_size_)
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
