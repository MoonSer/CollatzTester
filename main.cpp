#include "source/ThreadPool.h"
#include "source/FutureHolder.h"

#include <iostream>
#include <functional>
#include <numeric>
#include <chrono>
#include <map>
#include <fstream>

bool end = false;

void TotalPrinter(std::shared_ptr<ElementsHolder> holder)
{
    while (!end)
    {
        std::cout << "\rStat: " << holder->GetDoneCount() << "/" << holder->GetTotalCount() << "     " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

uint64_t Execute(std::function<void()> function, int times)
{
    std::vector<std::chrono::milliseconds> time;
    for (uint64_t i = 0; i < times; ++i)
    {
        auto now = std::chrono::steady_clock::now();
        function();
        time.emplace_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - now));
    }

    return std::accumulate(time.begin(), time.end(), std::chrono::milliseconds(0)).count() / time.size();
}
#include <mongocxx/client.hpp>
int main()
{
    // std::map<uint64_t, uint64_t> times;
    // for (uint64_t i = 4; i < 16; i += 2)
    // {
    //     auto time = Execute([&i]()
    //                         {
    // auto holder = std::make_shared<ElementsHolder>(std::deque<uint64_t>{9, 6, 1, 1, 1, 1, 2}, 10, 7);
    // auto holder = std::make_shared<ElementsHolder>(std::deque<uint64_t>{1, 2, 1}, 8, 3);
    // end = false;
    // std::thread t(&TotalPrinter, holder);
    // ThreadPool pool(4, holder);
    // pool.Execute();
    // end = true;
    // t.join();

    mongocxx::instance ins;
    mongocxx::client client(mongocxx::uri("mongodb://localhost:27017"));
    auto res = client["Collatz"]["Collatz"].find_one({});
    if (!res.has_value())
    {
        return 1;
    }
    for (auto i : res.get())
    {
        std::cout << i.key() << "\n";
    }
    //                         2);
    //     std::cout << "Threads: " << i << ". Average time: " << time << " ms\n";
    //     times.emplace(time, i);
    // }

    // std::cout << "Min: " << times.begin()->first << " ms  -  " << times.begin()->second << " threads\n";
    // // сохранять t для Цикла!!!
    // auto errors = FutureHolder::instance().GetErrors();
    // if (!errors.empty())
    // {
    //     std::ofstream out("out", std::ios_base::out);
    //     for (const auto &error : errors)
    //     {
    //         out << error << "\n";
    //     }
    // }

    return 0;
}