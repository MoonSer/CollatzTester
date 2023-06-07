#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

#include <string_view>
#include <memory>

#include "DatabaseInstance.h"

/// @brief Класс инициализирует БД
class Database
{
public:
    /// @brief Конструктор по умолчанию
    Database() noexcept;

    /// @brief Создаёт и возвращает указатель на DatabaseInstance
    /// @return указатель на DatabaseInstance
    std::unique_ptr<DatabaseInstance> GetInstance() noexcept;

public:
    static constexpr std::string_view database_name = "Collatz";
    static constexpr std::string_view table_name = "Collatz";

private:
    mongocxx::instance instance_;
    mongocxx::pool mongodb_pool_;
};

#endif