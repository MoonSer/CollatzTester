#include "Database.h"

#include <stdexcept>

Database::Database() noexcept
    : connect_future_(nullptr), cluster_(cass_cluster_new()),
      session_(cass_session_new()), last_error_("No error")
{
    cass_cluster_set_contact_points(cluster_, "127.0.0.1");
}

bool Database::SetUp() noexcept
{
    connect_future_ = cass_session_connect(session_, cluster_);
    return CheckResult(connect_future_);
}

bool Database::Insert(CollatzSolution &solution) noexcept
{
    CassStatement *statement = cass_statement_new("INSERT INTO Collatz (first_member, second_member, sqs_sequence, is_cycle) VALUES (?, ?, ?, ?);", 4);

    auto [firstMember, lastMember] = solution.GetCassResult();
    cass_statement_bind_decimal(statement, 0, firstMember.first, firstMember.second, 0);
    cass_statement_bind_decimal(statement, 1, lastMember.first, lastMember.second, 0);
    cass_statement_bind_bool(statement, 3, static_cast<cass_bool_t>(solution.IsSequence()));

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
