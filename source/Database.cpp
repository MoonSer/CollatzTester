#include "Database.h"

#include <stdexcept>

const std::string CREATE_KEYSPACE = "CREATE KEYSPACE IF NOT EXISTS Collatz WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 1 };";
const std::string USE_KEYSPACE = "USE Collatz;";
const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS Collatz( first_base decimal, first_addition decimal, second_base decimal, second_addition decimal, sqs_sequence frozen<list<bigint>>, is_cycle boolean, PRIMARY KEY((first_base, second_base, sqs_sequence, is_cycle)));";

Database::Database() noexcept
    : connect_future_(nullptr), cluster_(cass_cluster_new()),
      session_(cass_session_new()), last_error_("No error")
{
    cass_cluster_set_contact_points(cluster_, "127.0.0.1");
}

bool Database::SetUp() noexcept
{
    connect_future_ = cass_session_connect(session_, cluster_);
    if (CheckResult(connect_future_))
    {
        return SimpleExecute(CREATE_KEYSPACE) && SimpleExecute(USE_KEYSPACE) && SimpleExecute(CREATE_TABLE);
    }
    return false;
}

bool Database::Insert(const CollatzSolution &solution) noexcept
{
    CassStatement *statement = cass_statement_new("INSERT INTO Collatz (first_base, first_addition, second_base, second_addition, sqs_sequence, is_cycle) VALUES (?, ?, ?, ?, ?, ?);", 6);
    { // Prepare statement
        auto [firstMember, lastMember, is_sequence] = solution.GetCassResult();
        const auto &firstBase = firstMember.first;
        cass_statement_bind_decimal(statement, 0, firstBase.first.get(), firstBase.second, 0);

        const auto &firstAddition = firstMember.second;
        cass_statement_bind_decimal(statement, 1, firstAddition.first.get(), firstAddition.second, 0);

        const auto &lastBase = lastMember.first;
        cass_statement_bind_decimal(statement, 2, lastBase.first.get(), lastBase.second, 0);

        const auto &lastAddition = lastMember.second;
        cass_statement_bind_decimal(statement, 3, lastAddition.first.get(), lastAddition.second, 0);

        CassCollection *sqs_sequence = solution.GetCassSequense();
        cass_statement_bind_collection(statement, 4, sqs_sequence);
        cass_collection_free(sqs_sequence);

        cass_statement_bind_bool(statement, 5, static_cast<cass_bool_t>(is_sequence));
    }

    // Execute
    CassFuture *query_future = cass_session_execute(session_, statement);
    cass_statement_free(statement);

    bool executionResult = CheckResult(query_future);
    cass_future_free(query_future);

    return executionResult;
}

const std::string Database::GetLastError() const noexcept
{
    return last_error_;
}

bool Database::CheckResult(CassFuture *future) noexcept
{
    if (cass_future_error_code(future) != CASS_OK)
    {
        const char *message;
        size_t message_length;
        cass_future_error_message(future, &message, &message_length);
        last_error_ = message;
        return false;
    }
    return true;
}

bool Database::SimpleExecute(std::string_view query) noexcept
{
    CassStatement *statement = cass_statement_new(query.data(), 0);
    CassFuture *query_future = cass_session_execute(session_, statement);
    cass_statement_free(statement);

    bool executionResult = CheckResult(query_future);
    cass_future_free(query_future);

    return executionResult;
}

void Database::Select() noexcept
{
    CassStatement *statement = cass_statement_new("SELECT * FROM Collatz;", 0);
    CassFuture *query_future = cass_session_execute(session_, statement);
    cass_statement_free(statement);
    bool executionResult = CheckResult(query_future);
    if (!executionResult)
    {
        std::cout << this->last_error_ << "\n";
        return;
    }

    const CassResult *result = cass_future_get_result(query_future);

    const CassRow *row = cass_result_first_row(result);

    /* Now we can retrieve the column values from the row */
    const cass_byte_t *first;
    size_t size;
    cass_int32_t scale;

    /* Get the column value of "key" by name */
    cass_value_get_decimal(cass_row_get_column_by_name(row, "second_addition"), &first, &size, &scale);
    std::cout << size << '\n';
    // for (int i = 0; i < size; ++i)
    // {
    // std::cout << first[i] << "\n";
    // }

    mpz_class a;
    mpz_import(a.get_mpz_t(), 1, 1, size, 0, 0, first);
    std::cout << a << '\n';

    cass_result_free(result);
    cass_future_free(query_future);
}