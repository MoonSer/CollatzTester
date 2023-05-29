#pragma once
#ifndef COLLATZ_SOLUTION_H
#define COLLATZ_SOLUTION_H

#include <gmpxx.h>
#include <vector>
#include <iostream>
#include <cstdint>

class CollatzSolution
{
public:
    using CassResult = std::pair<std::unique_ptr<cass_byte_t[]>, size_t>;

    CollatzSolution(std::vector<uint64_t> &sqs_sequence)
        : sqs_sequence_(sqs_sequence) {}

    std::pair<mpz_class, mpz_class> FirstAndLast() const noexcept
    {
        mpz_class a = 2, b = 2;
        mpz_pow_ui(a.get_mpz_t(), a.get_mpz_t(), sqs_sequence_.front());
        mpz_pow_ui(b.get_mpz_t(), b.get_mpz_t(), sqs_sequence_.back());

        a *= base_.front();
        a -= 1;

        b *= base_.back();
        b -= 1;

        return std::make_pair(a, b);
    }

    mpz_class LastBase() const noexcept
    {
        return base_.back();
    }

    mpz_class LastAddition() const noexcept
    {
        return addition_.back();
    }

    void RecalculateSolution(mpz_class x, mpz_class y, mpz_class a, mpz_class b)
    {
        // If it first solution - just add data in vector
        if (base_.empty())
        {
            mpz_class cropModifier = std::move(GetCropModificator(x, b));

            x -= cropModifier * b;
            y -= cropModifier * a;

            base_.emplace_back(x);
            base_.emplace_back(y);

            addition_.emplace_back(b);
            addition_.emplace_back(a);
            return;
        }

        mpz_class cropModifier = std::move(GetCropModificator(y, a));
        // If second - need to recalculate for t
        for (int i = 0; i < base_.size(); ++i)
        {
            base_[i] += x * addition_[i];
            addition_[i] *= b;

            // crop
            base_[i] -= cropModifier * addition_[i];
        }

        y -= cropModifier * a;
        base_.push_back(y);
        addition_.push_back(a);
    }

    static mpz_class GetCropModificator(const mpz_class &x, const mpz_class &b)
    {
        mpz_class c;
        mpz_tdiv_q(c.get_mpz_t(), x.get_mpz_t(), b.get_mpz_t());
        if (x < 0)
        {
            if (b > 0)
            {
                c -= 1;
            }
            else
            {
                c += 1;
            }
        }
        return c;
    }

    std::tuple<CassResult, CassResult, bool> GetCassResult() const noexcept
    {
        mpz_class first_member = 2, last_member = 2;
        mpz_pow_ui(first_member.get_mpz_t(), first_member.get_mpz_t(), sqs_sequence_.front());
        mpz_pow_ui(last_member.get_mpz_t(), last_member.get_mpz_t(), sqs_sequence_.back());

        mpz_class divisible = addition_.back() * last_member - addition_.front() * first_member;

        first_member *= base_.front();
        last_member *= base_.back();

        first_member -= last_member;
        mpz_class divider = std::move(first_member);

        // @TODO Use m0 + m1 * t instead m0
        // @TODO Recreate table with t-value

        return std::make_tuple(GetCassResult(base_.back()), GetCassResult(last_member), mpz_divisible_p(divisible.get_mpz_t(), divider.get_mpz_t()) > 0);
    }

    bool IsSequence() const noexcept
    {
        mpz_class z = 2, q = 2;
        mpz_pow_ui(z.get_mpz_t(), z.get_mpz_t(), sqs_sequence_.front());
        mpz_pow_ui(q.get_mpz_t(), q.get_mpz_t(), sqs_sequence_.back());

        mpz_class divisible = addition_.back() * q - addition_.front() * z;
        mpz_class divider = base_.front() * z - base_.back() * q;

        return (mpz_divisible_p(divisible.get_mpz_t(), divider.get_mpz_t()) > 0);
    }

private:
    CassResult GetCassResult(const mpz_class &value) const noexcept
    {
        size_t size = mpz_size(value.get_mpz_t()) * mp_bits_per_limb / 8;

        std::unique_ptr<cass_byte_t[]> data(new cass_byte_t[size]);

        mpz_export(data.get(), nullptr, 1, size, 0, 0, value.get_mpz_t());
        return std::make_pair(data, size);
    }

private:
    std::vector<uint64_t> &sqs_sequence_;
    std::vector<mpz_class> base_;
    std::vector<mpz_class> addition_;
};

#endif