#pragma once
#ifndef COLLATZ_SOLUTION_H
#define COLLATZ_SOLUTION_H

#include <cassandra.h>
#include <deque>
#include <gmpxx.h>
#include <memory>
#include <cstdint>

/// @brief В этом типе хранится mpz_class сконвертированный в decimal для Cassandra
using CassDecimal = std::pair<std::unique_ptr<uint8_t[]>, size_t>;

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

    /// @brief Пересчитывает решение, добавляя коэффиценты в решение
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

    /// @brief Возвращает первый и последний коэффицент решения sqs-последовательности, сконвертированный в представление для Cassandra
    /// @return первый и последний коэффицент решения sqs-последовательности, сконвертированный в представление для Cassandra
    std::tuple<std::pair<CassDecimal, CassDecimal>, std::pair<CassDecimal, CassDecimal>, bool> GetCassResult() const noexcept;

    /// @brief Возвращает sqs-последовательность, сконвертированную в представление для Cassandra
    /// @return sqs-последовательность, сконвертированную в представление для Cassandra
    CassCollection *GetCassSequense() const noexcept;

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
    /// @brief Конвертирует mpz_class в CassDecimal
    /// @param[in] value переменная для конвертации
    /// @return CassDecimal - конвертированный mpz_class в вид, который понимает драйвер cassandra
    CassDecimal GetCassResult(const mpz_class &value) const noexcept;

private:
    /// @brief sqs-последовательность
    std::deque<uint64_t> &sqs_sequence_;
    /// @brief Основные коэффиценты решения
    std::deque<mpz_class> base_;
    /// @brief Дополнительные коэффиценты (множители с точностью до t)
    std::deque<mpz_class> addition_;
};

#endif