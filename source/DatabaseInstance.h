#pragma once
#ifndef DATABASE_INSTANCE_H
#define DATABASE_INSTANCE_H

#include <string>
#include <memory>
#include <set>
#include <mutex>
#include <condition_variable>

#include "CollatzSolution.h"

/// @brief Класс содержит указатель на сессию с БД и выполняет запросы
class DatabaseInstance
{
     friend class Database;

public:
     /// @brief Конструктор - сохраняет экземпляр сессии для дальнейшего использования
     /// @param session
     DatabaseInstance(CassSession *session);

     /// @brief Добавляет запись в CollatzSolution
     /// @param[in] solution Решение Диофантова уравнения
     /// @return true, если вставка выполнена успешно
     void Insert(CollatzSolution &&solution) noexcept;

     /// @brief Выполняет простой запрос к БД (без результата запроса и подготовки переменных, юзается дл создание таблицы и пр-ва имен)
     /// @param[in] query запрос для выполнения
     /// @return true, если вставка выполнена успешно
     std::pair<bool, std::string> SimpleExecute(std::string_view query) noexcept;

     /// @brief Возвращает последнюю вычисленную sqs-последовательность
     /// @return sqs-последовательность
     std::deque<uint64_t> GetMax() noexcept;

private:
     /// @brief Проверяет результат выполнения Запроса к Cassandra
     /// @param future future запроса
     /// @return true, если
     std::string CheckResult(CassFuture *future) noexcept;

private:
     CassSession *session_;
};

#endif