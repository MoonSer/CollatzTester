#ifndef COLLATZ_SOLVER_H
#define COLLATZ_SOLVER_H

#include "CollatzSolution.h"

#include <vector>
#include <optional>
#include <cstdint>

#define LOG

class CollatzSolver
{
public:
    CollatzSolver(std::vector<uint64_t> &&sqs_sequence);

    CollatzSolution Solve() noexcept;

private:
    std::tuple<mpz_class, mpz_class, mpz_class> GetCoeffs_() const noexcept;

private:
    CollatzSolution solution_;
    std::vector<uint64_t> sqs_sequence_;
    mutable std::vector<uint64_t>::iterator iter_;
}; // CollatzSolver

#endif