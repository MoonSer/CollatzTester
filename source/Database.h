#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <cassandra.h>
#include <utility>
#include <memory>
#include <string>

#include "CollatzSolution.h"

/// @brief Класс выполняет запросы и иницализирует Базу Данных
class Database
{
public:
    /// @brief Конструктор, устанавливает настройки соединения
    Database() noexcept;

    bool SetUp() noexcept;

    bool Insert(CollatzSolution &solution) noexcept;

    const std::string GetLastError() const noexcept;

private:
    bool CheckResult(CassFuture *future) noexcept;

private:
    CassFuture *connect_future_;
    CassCluster *cluster_;
    CassSession *session_;

    std::string last_error_;
};

#endif