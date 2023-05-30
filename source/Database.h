#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <cassandra.h>
#include <string>
#include <memory>
#include <utility>

#include "CollatzSolution.h"

/// @brief Класс выполняет запросы и иницализирует Базу Данных
class Database
{
public:
    /// @brief Конструктор, устанавливает настройки соединения
    Database() noexcept;

    /// @brief Выполняет подключение к Cassandra
    /// @return true, если соединение успешно
    bool SetUp() noexcept;

    /// @brief Добавляет запись с CollatzSolution
    /// @param[in] solution Решение диофантнова уравнения для добавления
    /// @return true, если вставка выполнена успешно
    bool Insert(const CollatzSolution &solution) noexcept;

    /// @brief Выполняет простой запрос к БД (без результата запроса и подготовки переменных)
    /// @param query запрос для выполнения
    /// @return true, если вставка выполнена успешно
    bool SimpleExecute(std::string_view query) noexcept;

    /// @brief Возврашает строку с последней возникшей ошибкой
    /// @return строка ошибки
    const std::string GetLastError() const noexcept;

    void Select() noexcept;

private:
    /// @brief Проверяет результат выполнения Запроса к Cassandra
    /// @param future future запроса
    /// @return true, если
    bool CheckResult(CassFuture *future) noexcept;

private:
    CassFuture *connect_future_;
    CassCluster *cluster_;
    CassSession *session_;

    std::string last_error_;
};

#endif