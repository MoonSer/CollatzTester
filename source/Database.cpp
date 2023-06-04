#include "Database.h"

#include <cassandra.h>

#include "DatabaseInstance.h"

const std::string BATCH_BEGIN = "BEGIN BATCH;", BATCH_END = "APPLY BATCH;";
const std::string CREATE_KEYSPACE = "CREATE KEYSPACE IF NOT EXISTS Collatz WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 1 };";
const std::string USE_KEYSPACE = "USE Collatz;";
const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS Collatz( first_base decimal, first_addition decimal, second_base decimal, second_addition decimal, sqs_sequence frozen<list<bigint>>, is_cycle boolean, PRIMARY KEY(is_cycle, sqs_sequence));";

Database::Database() noexcept
    : cluster_(cass_cluster_new()), session_(cass_session_new())
{
    cass_cluster_set_contact_points(cluster_, "127.0.0.1");
    // cass_cluster_set
}

std::pair<bool, std::string> Database::SetUp() noexcept
{
    connect_future_ = cass_session_connect(session_, cluster_);

    std::unique_ptr<DatabaseInstance> database = GetInstance();
    std::string connectionResult = database->CheckResult(connect_future_);
    if (!connectionResult.empty())
        return std::make_pair(false, connectionResult);

    auto [status1, error1] = database->SimpleExecute(CREATE_KEYSPACE);
    if (!status1)
    {
        return std::make_pair(false, error1);
    }

    auto [status2, error2] = database->SimpleExecute(USE_KEYSPACE);
    if (!status2)
    {
        return std::make_pair(false, error2);
    }

    auto [status3, error3] = database->SimpleExecute(CREATE_TABLE);
    if (!status3)
    {
        return std::make_pair(false, error3);
    }
    return std::make_pair(true, std::string(""));
}

std::unique_ptr<DatabaseInstance> Database::GetInstance() noexcept
{
    return std::make_unique<DatabaseInstance>(session_);
}