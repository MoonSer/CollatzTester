#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>

#include "DatabaseInstance.h"

/// @brief Класс инициализирует БД и создаёт указатели на сессии
class Database
{
public:
    /// @brief Конструктор, устанавливает настройки соединения
    Database() noexcept;

    /// @brief Выполняет подключение к Cassandra
    /// @return пара. ключ - true, если задача выполнена успешно. Если нет - то значением будет текст ошибки
    std::pair<bool, std::string> SetUp() noexcept;

    /// @brief Создаёт и возвращает указатель на DatabaseInstance
    /// @return указатель на DatabaseInstance
    std::unique_ptr<DatabaseInstance> GetInstance() noexcept;

private:
    CassFuture *connect_future_;
    CassCluster *cluster_;
    CassSession *session_;
};

#endif