#pragma once
#ifndef COLLATZ_SOLUTION_H
#define COLLATZ_SOLUTION_H

#include <gmpxx.h>
#include <memory>
#include <cstdint>
#include <deque>

#include <bsoncxx/document/value.hpp>
#include <bsoncxx/array/value.hpp>

/// @brief Класс представляет собой решение для sqs-последовательности Коллатца
class CollatzSolution
{
public:
    /// @brief Конструктор, сохраняет последовательность
    /// @param[in] sqs_sequence sqs-последовательность паттерна коллатца
    CollatzSolution(std::deque<uint64_t> &sqs_sequence);

    /// @brief Возвращает последний сохранённый m_k коэффициент
    /// @return последний сохранённый m_k коэффициент
    const mpz_class &LastBase() const noexcept;

    /// @brief Возвращает последний сохранённый m_{k_1} коэффициент (при t)
    /// @return последний сохранённый m_{k_1} коэффициент (при t)
    const mpz_class &LastAddition() const noexcept;

    /// @brief Пересчитывает решение, добавляя коэффициенты в решение
    /// @param[in] x m_0
    /// @param[in] y m_1
    /// @param[in] a m_{0_1}
    /// @param[in] b m_{1_1}
    void RecalculateSolution(mpz_class &&x, mpz_class &&y, mpz_class &&a, mpz_class &&b) noexcept;

    /// @brief Возвращает множитель для сокращения чисел x и b (такое c, что: |x - bc| < b)
    /// @param[in] x m_0
    /// @param[in] b m_{0_1}
    /// @return множитель для сокращения чисел x и b (такое c, что: |x - bc| < b)
    static mpz_class GetCropModificator(const mpz_class &x, const mpz_class &b);

    /// @brief Возвращает результат решения в виде, который понимает mongodb
    /// @return результат решения в виде, который понимает mongodb
    bsoncxx::document::value GetMongoResult() const noexcept;

    friend std::ostream &operator<<(std::ostream &s, const CollatzSolution &solution)
    {
        s << "Sequence:\n";
        for (const auto &v : solution.sqs_sequence_)
        {
            s << v << " ";
        }
        s << "\nSolution:\n";
        for (int i = 0; i < solution.base_.size(); ++i)
        {
            s << solution.base_[i] << " + " << solution.addition_[i] << "t"
              << "; ";
        }
        s << "\n";
        return s;
    }

private:
    bsoncxx::array::value GetMongoSequence() const noexcept;

private:
    /// @brief sqs-последовательность
    std::deque<uint64_t> &sqs_sequence_;
    /// @brief Основные коэффициенты решения
    std::deque<mpz_class> base_;
    /// @brief Дополнительные коэффициенты (множители с точностью до t)
    std::deque<mpz_class> addition_;
};

#endif