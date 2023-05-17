/*
#include <iostream>
#include <tuple>
#include <gmp.h>
#include <gmpxx.h>
#include <optional>
#include <thread>

std::ostream &operator<<(std::ostream &s, std::tuple<mpz_class, mpz_class, mpz_class> t) {
    s << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t);
    return s;
}

struct DiophSolution {
    DiophSolution(mpz_class x, mpz_class y, mpz_class free_x, mpz_class free_y) 
    : x(x), y(y), free_x(free_x), free_y(free_y) 
    {
        if (abs(x) > abs(free_x))
            this->x -= free_x * (x/free_x);
        if (abs(y) > abs(free_y))
            this->y -= free_y * (y/free_y);
    }
    mpz_class x;
    mpz_class y;
    mpz_class free_x;
    mpz_class free_y;
};

std::ostream &operator<<(std::ostream &s, const DiophSolution& solution) {
    s << solution.x << " + " << solution.free_x << "t" << "; " << solution.y << " + " << solution.free_y << "t";
    return s;
}

// /// @brief Solve diophant equation ax + by = c
// /// @return if solvable - return solution else nullopt
// std::optional<DiophSolution> GetSolution(mpz_class a, mpz_class b, mpz_class c) {
//     mpz_class x, y, g;
//     mpz_gcdext(g.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());

//     if (c % g != 0)
//         return std::nullopt;
    
//     mpz_class coeff = c/g;
//     return DiophSolution{x * coeff, y*coeff, b/g, a/g};
// }

std::tuple<mpz_class, mpz_class, mpz_class> GetCoeffs(uint64_t s1, uint64_t q1, uint64_t s2) {
    mpz_class m1_powed(2);
    mpz_pow_ui(m1_powed.get_mpz_t(), m1_powed.get_mpz_t(), q1+s2);

    mpz_class m2_powed(-3);
    mpz_pow_ui(m2_powed.get_mpz_t(), m2_powed.get_mpz_t(), s1);
    
    mpz_class free_powed(2);
    mpz_pow_ui(free_powed.get_mpz_t(), free_powed.get_mpz_t(), q1);

    return {m1_powed, m2_powed, free_powed-1};
}



int main() {
    auto [a, b, c] = GetCoeffs(5, 5, 5);
    
    // auto [a, b, c] = GetCoeffs(3, 2, 3);
    // std::cout << a << " " << b <<" " << c << " Sol:" << GetSolution(a, b, c).value();

    
    return 0;
}*/
#include <cstdint>
#include <iostream>
#include <vector>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

int main() {
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::client client{mongocxx::uri("mongodb://127.0.0.1:27017")};

    auto db = client["mydb"];
    auto collection = db["test"];

    
    std::cout << collection.name() << "\n";
    for (auto doc : collection.find({}))
        std::cout << bsoncxx::to_json(doc, bsoncxx::ExtendedJsonMode::k_relaxed) << std::endl;
    std::cout << std::endl;
    return 0;
}