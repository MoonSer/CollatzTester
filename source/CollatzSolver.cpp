#include "CollatzSolver.h"

#include <iostream>

CollatzSolver::CollatzSolver(const std::vector<uint64_t> &sqs_sequence)
    : sqs_sequence_(sqs_sequence), solution_(sqs_sequence_)
{
    if (sqs_sequence_.size() < 3 || sqs_sequence_.size() % 2 == 0)
    {
        throw std::runtime_error("Sqs sequence len must be more or equal than 3 and odd!");
    }
    iter_ = sqs_sequence_.begin();
}

std::optional<CollatzSolution> CollatzSolver::Solve() noexcept
{
    while (iter_ != --sqs_sequence_.end())
    {
        mpz_class x, y, g;
        auto [a, b, c] = GetCoeffs_();

        // Solve dioph equation and get x,y coeffs: ax + by = g
        mpz_gcdext(g.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());

        if (c % g != 0)
        {
            return std::nullopt;
        }
        // mpz_abs(b.get_mpz_t(), b.get_mpz_t());

        c /= g;
        x *= c;
        y *= c;

        b /= g;
        a /= -g;

        // Save solution
        solution_.RecalculateSolution(std::move(x), std::move(y), std::move(a), std::move(b));
    }
    return solution_;
}

std::tuple<mpz_class, mpz_class, mpz_class> CollatzSolver::GetCoeffs_() const noexcept
{
    bool is_first = (iter_ == sqs_sequence_.begin());
    mpz_class a = 3, b = 2, c = 2;
    uint64_t s1 = *iter_;
    uint64_t q1 = *(++iter_);
    uint64_t s2 = *(++iter_);

    mpz_pow_ui(a.get_mpz_t(), a.get_mpz_t(), s1);
    mpz_pow_ui(b.get_mpz_t(), b.get_mpz_t(), q1 + s2);
    mpz_pow_ui(c.get_mpz_t(), c.get_mpz_t(), q1);

    if (is_first)
    {
        return std::make_tuple(a, -b, 1 - c);
    }
    return std::make_tuple(a * solution_.LastAddition(), -b, 1 - c - a * solution_.LastBase());
}