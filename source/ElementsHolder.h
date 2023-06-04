#pragma once
#ifndef ELEMENTS_HOLDER_H
#define ELEMENTS_HOLDER_H

#include <cstdint>
#include <gmpxx.h>
#include <deque>

/// @brief Класс генерирует sqs-последовательности
class ElementsHolder
{
public:
    /// @brief Конструктор
    /// @param[in] from Последовательность, с которой необходимо начинать итерацию
    /// @param[in] max_item максимальное значение каждого члена последовательности
    /// @param[in] max_size максимальная длина последовательности
    ElementsHolder(std::deque<uint64_t> &&from, uint64_t max_item, uint64_t max_size) noexcept;

    /// @brief Возвращает следующую sqs-последовательность
    /// @return следующую sqs-последовательность
    std::deque<uint64_t> Next() noexcept;

    /// @brief Проверяет, есть ли ещё sqs-последовательность
    /// @return true, если есть ещё последовательность
    bool HasNext() const noexcept;

    /// @brief Возвращает общее кол-во последовательностей, которое можно сгенерировать по начальным данным
    /// @return общее кол-во последовательностей, которое можно сгенерировать по начальным данным
    const mpz_class &GetTotalCount() const noexcept;

    /// @brief Возвращает сгенерированное кол-во последовательностей
    /// @return сгенерированное кол-во последовательностей
    const mpz_class &GetDoneCount() const noexcept;

private:
    /// @brief Текущая последовательность
    std::deque<uint64_t> current_sequence_;
    /// @brief Максимальное значение элемента последовательности
    uint64_t max_item_;
    /// @brief Максимальная длина последовательности
    uint64_t max_sequence_size_;
    /// @brief Общее количество последовательностей, которое можно сгенерировать по начальным данным
    mpz_class work_total_;
    /// @brief Сгенерированное кол-во последовательностей
    mpz_class work_done_;
};

#endif // ELEMENTS_HOLDER_H