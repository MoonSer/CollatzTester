#include "DatabaseInstance.h"

#include <mongocxx/client.hpp>

DatabaseInstance::DatabaseInstance(mongocxx::pool::entry &&pool_entry) noexcept
    : pool_entry_(std::move(pool_entry))
{
     documents_pool_.reserve(300);
}

DatabaseInstance::~DatabaseInstance()
{
     if (!documents_pool_.empty())
     {
          auto result = (*pool_entry_)["Collatz"]["Collatz"].insert_many(documents_pool_);

          if (!result.has_value())
          {
               std::cout << "Err. Result does not exists!\n";
          }
          else if (result.has_value() && result.get().inserted_count() != documents_pool_.size())
          {
               std::cout << "Err. Inserted: " << result.get().inserted_count() << ". Size: " << documents_pool_.size() << "\n";
          }
          documents_pool_.clear();
     }
}

void DatabaseInstance::Insert(CollatzSolution &&solution) noexcept
{
     std::cout << "Inserting: " << solution << "\n";
     documents_pool_.emplace_back(solution.GetMongoResult());

     if (documents_pool_.size() == 300)
     {
          auto result = (*pool_entry_)["Collatz"]["Collatz"].insert_many(documents_pool_);
          if (!result.has_value())
          {
               std::cout << "Err. Result does not exists!\n";
          }
          else if (result.has_value() && result.get().inserted_count() != documents_pool_.size())
          {
               std::cout << "Err. Inserted: " << result.get().inserted_count() << ". Size: " << documents_pool_.size() << "\n";
          }
          documents_pool_.clear();
     }
}

std::deque<uint64_t> DatabaseInstance::GetMax() noexcept
{
     // return {};
     // CassStatement *statement = cass_statement_new("SELECT MAX(sqs_sequence) FROM Collatz WHERE is_cycle IN (false, true);", 0);
     // CassFuture *query_future = cass_session_execute(session_, statement);
     // cass_statement_free(statement);

     // auto error_string = CheckResult(query_future);
     // if (!error_string.empty())
     // {
     //      return {};
     // }

     // const CassResult *result = cass_future_get_result(query_future);
     // const CassRow *row = cass_result_first_row(result);
     // const CassValue *value = cass_row_get_column(row, 0);

     // std::deque<uint64_t> values;
     // if (value != nullptr)
     // {
     //      auto number_collector = [](const int count, const cass_byte_t *array) -> uint64_t
     //      {
     //           uint64_t value = array[0];
     //           for (int i = 1; i < count; ++i)
     //           {
     //                value <<= 8;
     //                value += array[i];
     //           }
     //           return value;
     //      };
     //      const cass_byte_t *first;
     //      size_t size;
     //      cass_value_get_bytes(value, &first, &size);

     //      size = number_collector(4, first);

     //      uint64_t i = 4;
     //      for (; size > 0; --size)
     //      {
     //           i += 4;
     //           values.emplace_back(number_collector(8, first + i));
     //           i += 8;
     //      }
     // }

     // // mpz_import(a.get_mpz_t(), 1, 1, size, 0, 0, first);

     // cass_result_free(result);
     // cass_future_free(query_future);

     // if (value == nullptr)
     // {
     //      values = {1, 1, 0};
     // }
     // return values;
}