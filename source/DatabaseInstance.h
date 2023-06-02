#pragma once
#ifndef DATABASE_INSTANCE_H
#define DATABASE_INSTANCE_H

#include <string>
#include <memory>
#include <utility>

#include "CollatzSolution.h"

/// @brief Класс содержит указатель на сессию с БД и выполняет запросы
class DatabaseInstance
{
     friend class Database;

public:
     /// @brief Конструктор - сохраняет экземпляр сессии для дальнейшего использования
     /// @param session
     DatabaseInstance(CassSession *session);

     /// @brief Добавляет запись с CollatzSolution
     /// @param[in] solution Решение Диофантова уравнения для добавления
     /// @return true, если вставка выполнена успешно
     bool Insert(CollatzSolution &&solution) noexcept;

     /// @brief Выполняет простой запрос к БД (без результата запроса и подготовки переменных)
     /// @param query запрос для выполнения
     /// @return true, если вставка выполнена успешно
     bool SimpleExecute(std::string_view query) noexcept;

     /// @brief Возвращает строку с последней возникшей ошибкой
     /// @return строка ошибки
     const std::string GetLastError() const noexcept;

     /// @brief Возвращает последнюю вычисленную sqs-последовательность
     /// @return sqs-последовательность
     std::vector<uint64_t> GetMax() noexcept;

private:
     /// @brief Проверяет результат выполнения Запроса к Cassandra
     /// @param future future запроса
     /// @return true, если
     bool CheckResult(CassFuture *future) noexcept;

private:
     CassSession *session_;
     std::string last_error_;
};

#endif