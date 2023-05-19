#include "Database.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>

std::string_view Database::database_name = "COLLATZ_DB";

Database::Database()
    : mongodb_instance_(), mongodb_pool_({mongocxx::uri{}})
{
}

mongocxx::pool::entry Database::Pool() noexcept
{
    return mongodb_pool_.acquire();
}