// #include "source/CollatzSolver.h"

// bool Collatz(mpz_class start, mpz_class end)
// {
//     int i = 10000000;
//     while (start != end && i > 0)
//     {
//         if (start % 2 == 0)
//         {
//             start /= 2;
//         }
//         else
//         {
//             start *= 3;
//             start += 1;
//         }
//         i -= 1;
//     }
//     if (i == 0)
//     {
//         return false;
//     }
//     return true;
// }

// CollatzSolver s({3, 2, 3, 1, 3});
// s.Solve();
// auto now = std::chrono::steady_clock::now();
// CollatzSolver s({213, 231, 123});
// auto [from, to] = s.Solve().value().solution();
// std::cout << (std::chrono::steady_clock::now() - now).count() << "\n";

// now = std::chrono::steady_clock::now();
// if (!Collatz(from, to))
// {
//     std::cout << "Err\n";
// }
// std::cout << (std::chrono::steady_clock::now() - now).count() << "\n";

// #include <gmp.h>
// #include <gmpxx.h>
// #include <vector>
// #include <cassandra.h>
// #include <iostream>
#include <string_view>
#include "source/Database.h"
constexpr std::string_view KeySpaceCreate = "CREATE KEYSPACE Collatz with replication = {'class':'SimpleStrategy', 'replication_factor' : 1};";
constexpr std::string_view KeySpaceUse = "USE Collatz;";
constexpr std::string_view CreateTable = "CREATE TABLE IF NOT EXISTS Collatz( first_base decimal, first_addition decimal, second_base decimal, second_addition decimal, sqs_sequence list<bigint>, is_cycle boolean, PRIMARY KEY((first_base, second_base, sqs_sequence, is_cycle)));";
constexpr std::string_view InsertQuery = "INSERT INTO Collatz (first_base, first_addition, second_base, second_addition, sqs_sequence, is_cycle) VALUES (?, ?, ?, ?, ?, ?);";
constexpr std::string_view Query = "SELECT * FROM Tester;";

// std::vector<cass_byte_t> mpz_to_cass(const mpz_class &number)
// {
//     size_t size = mpz_size(number.get_mpz_t()) * mp_bits_per_limb / 8;

//     std::vector<cass_byte_t> data;
//     data.resize(size);

//     mpz_export(data.data(), nullptr, 1, size, 0, 0, number.get_mpz_t());
//     return data;
// }

// void insert_query(CassSession *session, const std::string_view query)
// {
//     CassStatement *statement = cass_statement_new(query.data(), 2);

//     auto t1 = mpz_to_cass(mpz_class("123456"));
//     cass_statement_bind_decimal(statement, 0, t1.data(), t1.size(), 0);

//     auto t2 = mpz_to_cass(mpz_class("12345678"));
//     cass_statement_bind_decimal(statement, 1, t2.data(), t2.size(), 0);

//     CassFuture *query_future = cass_session_execute(session, statement);

//     /* Statement objects can be freed immediately after being executed */
//     cass_statement_free(statement);

//     /* This will block until the query has finished */
//     CassError rc = cass_future_error_code(query_future);

//     std::cout << "Query result: " << cass_error_desc(rc) << "\n";

//     cass_future_free(query_future);
// }

// void query_result(CassSession *session, const std::string_view query)
// {
//     CassStatement *statement = cass_statement_new(query.data(), 0);
//     CassFuture *query_future = cass_session_execute(session, statement);
//     /* This will also block until the query returns */
//     const CassResult *result = cass_future_get_result(query_future);

//     /* If there was an error then the result won't be available */
//     if (result == NULL)
//     {

//         /* Handle error */

//         cass_future_free(query_future);
//         return;
//     }

//     /* The future can be freed immediately after getting the result object */
//     cass_future_free(query_future);

//     /* This can be used to retrieve the first row of the result */
//     const CassRow *row = cass_result_first_row(result);

//     /* Now we can retrieve the column values from the row */
//     const cass_byte_t *first;
//     size_t size;
//     cass_int32_t scale;

//     /* Get the column value of "key" by name */
//     cass_value_get_decimal(cass_row_get_column_by_name(row, "first_member"), &first, &size, &scale);
//     for (int i = 0; i < size; ++i)
//     {
//         std::cout << int(first[i]) << "\n";
//     }

//     mpz_class a;
//     mpz_import(a.get_mpz_t(), 1, 1, size, 0, 0, first);
//     std::cout << "Readed: " << a << "\n";
//     /* This will free the result as well as the string pointed to by 'key' */
//     cass_result_free(result);
// }

// void mpz_to_cassandra()
// {
//     mpz_class t("123456");
//     std::cout << "Number: " << t << " " << mpz_size(t.get_mpz_t()) << " " << mp_bits_per_limb / 8 << "\n";
//     std::vector<cass_byte_t> decimal;
//     decimal.resize(mpz_size(t.get_mpz_t()) * mp_bits_per_limb / 8);

//     std::cout << "Exported: " << decimal.size() << "\n";
//     mpz_export(decimal.data(), nullptr, 1, mpz_size(t.get_mpz_t()) * mp_bits_per_limb / 8, 0, 0, t.get_mpz_t());
//     std::cout << sizeof(cass_byte_t) << "\n";
//     for (const auto &d : decimal)
//     {
//         std::cout << uint(d) << "\n";
//     }

//     mpz_class t2;
//     mpz_import(t2.get_mpz_t(), 1, 1, mpz_size(t.get_mpz_t()) * mp_bits_per_limb / 8, 0, 0, decimal.data());
//     std::cout << "Imported: " << t2 << " " << (t == t2) << "\n";
// }
#include "source/ThreadPool.h"

int main()
{
    ThreadPool pool(5, std::make_unique<ElementsHolder>(std::vector<uint64_t>{1, 1, 1}, 5, 5));
    pool.Execute();
    // std::vector<uint64_t> q = {2, 5, 6};
    // CollatzSolution s(q);
    // s.RecalculateSolution(mpz_class(7), mpz_class(6), mpz_class(32), mpz_class(27));

    // Database d;
    // auto [is_done, error_msg] = d.SetUp();
    // if (!is_done)
    // {
    //     std::cout << error_msg << "\n";
    //     return 1;
    // }
    // auto session = d.GetInstance();
    // session->Insert(s);
    // const auto value = session->GetMax();
    // for (const auto &i : value)
    //     std::cout << i << "\n";
    // /* Setup and connect to cluster */
    // CassFuture *connect_future = NULL;
    // CassCluster *cluster = cass_cluster_new();
    // CassSession *session = cass_session_new();

    // /* Add contact points */
    // cass_cluster_set_contact_points(cluster, "127.0.0.1");

    // /* Provide the cluster object as configuration to connect the session */
    // connect_future = cass_session_connect(session, cluster);

    // /* This operation will block until the result is ready */
    // CassError rc = cass_future_error_code(connect_future);

    // if (rc != CASS_OK)
    // {
    //     /* Display connection error message */
    //     const char *message;
    //     size_t message_length;
    //     cass_future_error_message(connect_future, &message, &message_length);
    //     std::cout << stderr << " Connect error: " << message_length << " " << message << "\n";
    // }

    // execute_query(session, KeySpaceUse);
    // query_result(session, Query);
    // // execute_query(session, CreateTable);
    // // insert_query(session, InsertQuery);

    // cass_future_free(connect_future);
    // cass_session_free(session);
    // cass_cluster_free(cluster);

    return 0;
}