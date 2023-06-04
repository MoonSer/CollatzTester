#ifndef COLLATZ_SOLVER_H
#define COLLATZ_SOLVER_H

#include "CollatzSolution.h"

#include <deque>
#include <cstdint>

/// @brief Класс находит коэффиценты для заданной sqs-последовательности задачи Коллатца
class CollatzSolver
{
public:
    /// @brief Конструктор. Сохраняет sqs-последовательность
    /// @param[in] sqs_sequence sqs-последовательность, для которой необходимо найти решение
    CollatzSolver(std::deque<uint64_t> &&sqs_sequence);

    /// @brief Класс возвращает решение для sqs-последовательности, заданной в конструкторе
    /// @return решение для sqs-последовательности
    CollatzSolution Solve() noexcept;

private:
    /// @brief Возвращает коэффиценты диофантового уравнения
    /// @return коэффиценты для диофантового уравнения
    ///
    /// Коэффиценты генерируются для sqs-паттерна, соответствующего поожению итератора (iter_). Сдвигает iter_ на два элемента вправо.
    std::tuple<mpz_class, mpz_class, mpz_class> GetCoeffs_() const noexcept;

private:
    /// @brief Решение является полем класа, потому что коэффиценты диофантового уравнения зависят от решения (GetCoeffs_)
    CollatzSolution solution_;
    /// @brief Последовательность, для которой необходимо найти решение
    std::deque<uint64_t> sqs_sequence_;
    /// @brief Итератор для sqs_sequence_ - указывает на следующий sqs-паттерн для вычисления
    mutable std::deque<uint64_t>::iterator iter_;
}; // CollatzSolver

#endif