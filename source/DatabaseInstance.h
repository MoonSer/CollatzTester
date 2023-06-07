#pragma once
#ifndef DATABASE_INSTANCE_H
#define DATABASE_INSTANCE_H

#include <mongocxx/pool.hpp>
#include <bsoncxx/document/value.hpp>

#include <string>
#include <string_view>
#include <memory>

#include "CollatzSolution.h"

/// @brief Класс содержит указатель на сессию с БД и выполняет запросы
class DatabaseInstance
{
public:
     /// @brief Конструктор - сохраняет экземпляр сессии для дальнейшего использования
     /// @param pool_entry экземпляр клиента из пула
     DatabaseInstance(mongocxx::pool::entry &&pool_entry) noexcept;

     ~DatabaseInstance();

     /// @brief Добавляет запись в CollatzSolution
     /// @param[in] solution Решение Диофантова уравнения
     /// @return true, если вставка выполнена успешно
     void Insert(CollatzSolution &&solution) noexcept;

     /// @brief Возвращает последнюю вычисленную sqs-последовательность
     /// @return sqs-последовательность
     std::deque<uint64_t> GetMax() noexcept;

private:
     mongocxx::pool::entry pool_entry_;
     std::vector<bsoncxx::document::value> documents_pool_;
};

#endif