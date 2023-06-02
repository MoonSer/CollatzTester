#pragma once
#ifndef COLLATZ_SOLUTION_H
#define COLLATZ_SOLUTION_H

#include <cassandra.h>

#include <gmpxx.h>
#include <vector>
#include <iostream>
#include <cstdint>
#include <memory>
#include <memory>

/// @brief В этом типе хранятся данные экспорта из gmp (mpc_class) в decimal тип для Cassandra
using CassDecimal = std::pair<std::unique_ptr<uint8_t[]>, size_t>;
/// @brief Класс представляет собой решение для sqs-последовательности Коллатца
class CollatzSolution
{
public:
    /// @brief Конструктор, сохраняет последовательность
    /// @param[in] sqs_sequence sqs-последовательность паттерна коллатца
    CollatzSolution(std::vector<uint64_t> &sqs_sequence);

    /// @brief Возвращает последний сохранённый m_k коэффициент
    /// @return последний сохранённый m_k коэффициент
    const mpz_class &LastBase() const noexcept;

    /// @brief Возвращает последний сохранённый m_{k_1} коэффициент (при t)
    /// @return последний сохранённый m_{k_1} коэффициент (при t)
    const mpz_class &LastAddition() const noexcept;

    /// @brief Добавляет переменные в решение
    /// @param x m_0
    /// @param y m_1
    /// @param a m_{0_1}
    /// @param b m_{1_1}
    void RecalculateSolution(mpz_class &&x, mpz_class &&y, mpz_class &&a, mpz_class &&b) noexcept;

    /// @brief Возвращает множитель для сокращения чисел x и b (такое c, что: |x - bc| < b)
    /// @param x m_0
    /// @param b m_{0_1}
    /// @return множитель для сокращения чисел x и b (такое c, что: |x - bc| < b)
    static mpz_class GetCropModificator(const mpz_class &x, const mpz_class &b);

    std::tuple<std::pair<CassDecimal, CassDecimal>, std::pair<CassDecimal, CassDecimal>, bool> GetCassResult() const noexcept;

    CassCollection *GetCassSequense() const noexcept;

    friend std::ostream &operator<<(std::ostream &s, const CollatzSolution &solution)
    {
        std::lock_guard<std::mutex> guard(CollatzSolution::m_);
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
        s << "\n"
          << std::flush;
        return s;
    }

private:
    /// @brief Конвертирует mpz_class в CassDecimal
    /// @param[in] value переменная для конвертации
    /// @return CassDecimal - конвертированный mpz_class в вид, который понимает драйвер cassandra
    CassDecimal GetCassResult(const mpz_class &value) const noexcept;

private:
    std::vector<uint64_t> &sqs_sequence_;
    std::vector<mpz_class> base_;
    std::vector<mpz_class> addition_;
    static std::mutex m_;
};

#endif