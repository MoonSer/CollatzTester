#pragma once
#ifndef ELEMENTS_HOLDER_H
#define ELEMENTS_HOLDER_H

#include <vector>
#include <cstdint>
#include <mutex>
#include <gmpxx.h>

class ElementsHolder
{
public:
    ElementsHolder(std::vector<uint64_t> &&from, uint64_t max_item, uint64_t max_size) noexcept;

    std::vector<uint64_t> Next() noexcept;

    bool HasNext() const noexcept;

    const mpz_class &GetTotalCount() const noexcept;

    const mpz_class &GetDoneCount() const noexcept;

private:
    mutable std::mutex mutex_;
    std::vector<uint64_t> current_;
    uint64_t max_item_;
    uint64_t max_size_;

    mpz_class work_total_;
    mpz_class work_done_;
};

#endif // ELEMENTS_HOLDER_H