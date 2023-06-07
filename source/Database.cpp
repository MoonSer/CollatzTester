#include "Database.h"

#include <mongocxx/client.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
// constexpr std::string_view Database::database_name = "Collatz";
// const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS Collatz( first_base decimal, first_addition decimal, second_base decimal, second_addition decimal, sqs_sequence frozen<list<bigint>>, cycle_coeff double, PRIMARY KEY(sqs_sequence, is_cycle));";

Database::Database() noexcept
    : mongodb_pool_(mongocxx::uri("mongodb://localhost:27017"))
{
    // mongocxx::client client(mongocxx::uri("mongodb://localhost:27017"));
    // client["Collatz"]["Collatz"].create_index(make_document(kvp("cycle_coeff", 1), kvp("sqs_sequence", 1)));
}

std::unique_ptr<DatabaseInstance> Database::GetInstance() noexcept
{
    return std::make_unique<DatabaseInstance>(std::move(mongodb_pool_.acquire()));
}