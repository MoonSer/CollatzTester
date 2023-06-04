#include "DatabaseInstance.h"

#include "FutureHolder.h"

DatabaseInstance::DatabaseInstance(CassSession *session)
    : session_(session)
{
}

void DatabaseInstance::Insert(CollatzSolution &&solution) noexcept
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

     FutureHolder::instance().PushWork(std::bind([this](CassStatement *statement) -> CassFuture *
                                                 {
          CassFuture *query_future = cass_session_execute(session_, statement);
          cass_statement_free(statement);
          return query_future; },
                                                 statement));
}

std::string DatabaseInstance::CheckResult(CassFuture *future) noexcept
{
     if (cass_future_error_code(future) != CASS_OK)
     {
          const char *message;
          size_t message_length;
          cass_future_error_message(future, &message, &message_length);
          return {message, message_length};
     }
     return "";
}

std::pair<bool, std::string> DatabaseInstance::SimpleExecute(std::string_view query) noexcept
{
     CassStatement *statement = cass_statement_new(query.data(), 0);
     CassFuture *query_future = cass_session_execute(session_, statement);
     cass_statement_free(statement);

     std::string executionResult = CheckResult(query_future);
     cass_future_free(query_future);

     return std::make_pair(executionResult.empty(), std::move(executionResult));
}

std::deque<uint64_t> DatabaseInstance::GetMax() noexcept
{
     CassStatement *statement = cass_statement_new("SELECT MAX(sqs_sequence) FROM Collatz WHERE is_cycle IN (false, true);", 0);
     CassFuture *query_future = cass_session_execute(session_, statement);
     cass_statement_free(statement);

     auto error_string = CheckResult(query_future);
     if (!error_string.empty())
     {
          return {};
     }

     const CassResult *result = cass_future_get_result(query_future);
     const CassRow *row = cass_result_first_row(result);
     const CassValue *value = cass_row_get_column(row, 0);

     std::deque<uint64_t> values;
     if (value != nullptr)
     {
          auto number_collector = [](const int count, const cass_byte_t *array) -> uint64_t
          {
               uint64_t value = array[0];
               for (int i = 1; i < count; ++i)
               {
                    value <<= 8;
                    value += array[i];
               }
               return value;
          };
          const cass_byte_t *first;
          size_t size;
          cass_value_get_bytes(value, &first, &size);

          size = number_collector(4, first);

          uint64_t i = 4;
          for (; size > 0; --size)
          {
               i += 4;
               values.emplace_back(number_collector(8, first + i));
               i += 8;
          }
     }

     // mpz_import(a.get_mpz_t(), 1, 1, size, 0, 0, first);

     cass_result_free(result);
     cass_future_free(query_future);

     if (value == nullptr)
     {
          values = {1, 1, 0};
     }
     return values;
}