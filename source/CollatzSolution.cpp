#include "CollatzSolution.h"

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

CollatzSolution::CollatzSolution(std::deque<uint64_t> &sqs_sequence)
    : sqs_sequence_(sqs_sequence) {}

const mpz_class &CollatzSolution::LastBase() const noexcept
{
     return base_.back();
}

const mpz_class &CollatzSolution::LastAddition() const noexcept
{
     return addition_.back();
}

void CollatzSolution::RecalculateSolution(mpz_class &&x, mpz_class &&y, mpz_class &&a, mpz_class &&b) noexcept
{
     // If it first solution - just add data in vector
     if (base_.empty())
     {
          mpz_class cropModifier = std::move(GetCropModificator(x, b));

          x -= cropModifier * b;
          y -= cropModifier * a;

          base_.push_back(std::move(x));
          base_.push_back(std::move(y));

          addition_.push_back(std::move(b));
          addition_.push_back(std::move(a));
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
     cropModifier *= a;
     y -= cropModifier;

     base_.push_back(std::move(y));
     addition_.push_back(std::move(a));
}

mpz_class CollatzSolution::GetCropModificator(const mpz_class &x, const mpz_class &b)
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

bsoncxx::document::value CollatzSolution::GetMongoResult() const noexcept
{
     mpz_class s1_exp = 2, sn_exp = 2;
     mpz_pow_ui(s1_exp.get_mpz_t(), s1_exp.get_mpz_t(), sqs_sequence_.front());
     mpz_pow_ui(sn_exp.get_mpz_t(), sn_exp.get_mpz_t(), sqs_sequence_.back());

     mpf_class divisible(std::move(addition_.back() * sn_exp));
     divisible -= addition_.front() * s1_exp;

     s1_exp *= base_.front();
     sn_exp *= base_.back();
     s1_exp -= sn_exp;

     mpf_class divider(std::move(s1_exp));
     mpf_div(divisible.get_mpf_t(), divisible.get_mpf_t(), divider.get_mpf_t());

     return make_document(
         kvp("first_base", base_.front().get_str()),
         kvp("firs_addition", addition_.front().get_str()),
         kvp("last_base", base_.back().get_str()),
         kvp("last_addition", addition_.back().get_str()),
         kvp("cycle_coeff", divisible.get_d()),
         kvp("sqs_sequence", GetMongoSequence()));
}

bsoncxx::array::value CollatzSolution::GetMongoSequence() const noexcept
{
     array sqs_sequence;
     for (const auto &sqs : sqs_sequence_)
     {
          sqs_sequence.append(bsoncxx::types::b_int64(sqs));
     }
     return sqs_sequence.extract();
}