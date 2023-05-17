#include "Database.h"

Database::Database()
    : database_name_("COLLATZ_DB"), mongodb_instance_(), mongodb_pool_({mongocxx::uri{}})
{
}

mongocxx::client &Database::pool() noexcept
{
    return *mongodb_pool_.acquire();
}
