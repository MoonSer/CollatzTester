#include "Database.h"

#include <cassandra.h>
#include <stdexcept>

#include "DatabaseInstance.h"

const std::string CREATE_KEYSPACE = "CREATE KEYSPACE IF NOT EXISTS Collatz WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 1 };";
const std::string USE_KEYSPACE = "USE Collatz;";
const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS Collatz( first_base decimal, first_addition decimal, second_base decimal, second_addition decimal, sqs_sequence frozen<list<bigint>>, is_cycle boolean, PRIMARY KEY(is_cycle, sqs_sequence));";

Database::Database() noexcept
    : connect_future_(nullptr), cluster_(cass_cluster_new()),
      session_(cass_session_new())
{
    cass_cluster_set_contact_points(cluster_, "127.0.0.1");
}

std::pair<bool, std::string> Database::SetUp() noexcept
{
    connect_future_ = cass_session_connect(session_, cluster_);

    std::unique_ptr<DatabaseInstance> database = GetInstance();

    bool is_success = database->CheckResult(connect_future_) &&
                      database->SimpleExecute(CREATE_KEYSPACE) &&
                      database->SimpleExecute(USE_KEYSPACE) &&
                      database->SimpleExecute(CREATE_TABLE);
    return {is_success, database->GetLastError()};
}

std::unique_ptr<DatabaseInstance> Database::GetInstance() noexcept
{
    return std::make_unique<DatabaseInstance>(session_);
}