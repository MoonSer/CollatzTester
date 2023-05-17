#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

class Database
{
public:
    Database();

    mongocxx::client &pool() noexcept;

private:
    std::string_view database_name_;
    mongocxx::instance mongodb_instance_;
    mongocxx::pool mongodb_pool_;
};

#endif