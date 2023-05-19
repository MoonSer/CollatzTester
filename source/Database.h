#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

class Database
{
public:
    Database();

    mongocxx::pool::entry Pool() noexcept;

private:
    static std::string_view database_name;
    mongocxx::instance mongodb_instance_;
    mongocxx::pool mongodb_pool_;
};

#endif