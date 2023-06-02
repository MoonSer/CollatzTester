#pragma once
#ifndef ELEMENTS_HOLDER_H
#define ELEMENTS_HOLDER_H

#include <vector>
#include <cstdint>

class ElementsHolder
{
public:
    ElementsHolder(std::vector<uint64_t> &&from, uint64_t max_item, uint64_t max_size) noexcept;

    std::vector<uint64_t> Next() noexcept;

    bool HasNext() const noexcept;

private:
    std::vector<uint64_t> current_;
    uint64_t max_item_;
    uint64_t max_size_;
};

#endif // ELEMENTS_HOLDER_H