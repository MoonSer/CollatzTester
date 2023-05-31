#include "DatabaseInstance.h"

DatabaseInstance::DatabaseInstance(CassSession *session)
    : session_(session), last_error_("No error")
{
}

bool DatabaseInstance::Insert(const CollatzSolution &solution) noexcept
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

const std::string DatabaseInstance::GetLastError() const noexcept
{
     return last_error_;
}

bool DatabaseInstance::CheckResult(CassFuture *future) noexcept
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

bool DatabaseInstance::SimpleExecute(std::string_view query) noexcept
{
     CassStatement *statement = cass_statement_new(query.data(), 0);
     CassFuture *query_future = cass_session_execute(session_, statement);
     cass_statement_free(statement);

     bool executionResult = CheckResult(query_future);
     cass_future_free(query_future);

     return executionResult;
}

void DatabaseInstance::Select() noexcept
{
     CassStatement *statement = cass_statement_new("SELECT * FROM Collatz WHERE is_cycle IN (false, true);", 0);
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
     cass_value_get_(cass_row_get_column_by_name(row, "sqs_sequence"), &first, &size, &scale);
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