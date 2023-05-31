#include "CollatzSolution.h"

#include <cassandra.h>

CollatzSolution::CollatzSolution(std::vector<uint64_t> &sqs_sequence)
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

std::tuple<std::pair<CassDecimal, CassDecimal>, std::pair<CassDecimal, CassDecimal>, bool> CollatzSolution::GetCassResult() const noexcept
{
     mpz_class s1_exp = 2, sn_exp = 2;
     mpz_pow_ui(s1_exp.get_mpz_t(), s1_exp.get_mpz_t(), sqs_sequence_.front());
     mpz_pow_ui(sn_exp.get_mpz_t(), sn_exp.get_mpz_t(), sqs_sequence_.back());

     mpz_class divisible = std::move(addition_.back() * sn_exp);
     divisible -= addition_.front() * s1_exp;

     s1_exp *= base_.front();
     sn_exp *= base_.back();
     s1_exp -= sn_exp;

     mpz_class divider = std::move(s1_exp);

     // @TODO Use m0 + m1 * t instead m0

     return std::make_tuple(std::make_pair(GetCassResult(base_.front()), GetCassResult(addition_.front())),
                            std::make_pair(GetCassResult(base_.back()), GetCassResult(addition_.back())),
                            (mpz_divisible_p(divisible.get_mpz_t(), divider.get_mpz_t()) > 0));
}

CassCollection *CollatzSolution::GetCassSequense() const noexcept
{
     CassCollection *cass_collection(cass_collection_new(CASS_COLLECTION_TYPE_LIST, sqs_sequence_.size()));

     for (const auto &sequence_member : sqs_sequence_)
     {
          cass_collection_append_int64(cass_collection, sequence_member);
     }
     return cass_collection;
}

CassDecimal CollatzSolution::GetCassResult(const mpz_class &value) const noexcept
{
     size_t size = mpz_size(value.get_mpz_t()) * mp_bits_per_limb / 8;

     std::unique_ptr<cass_byte_t[]> data(new cass_byte_t[size]);

     mpz_export(data.get(), nullptr, 1, size, 0, 0, value.get_mpz_t());
     return std::make_pair(std::move(data), size);
}