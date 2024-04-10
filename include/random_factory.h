#ifndef _RANDOM_FACTORY_H
#define _RANDOM_FACTORY_H

#include <random>
#include <vector>

namespace random_factory{
  using engine = std::mt19937_64;
  using result_type = engine::result_type;
  using distribution = std::uniform_int_distribution<result_type>;

  extern engine eng;
  
  void seed(result_type val);
  result_type gen(result_type min, result_type max);

  std::vector<size_t> k_random_nonrepeat(size_t k, size_t begin, size_t end);

  std::vector<size_t> k_random(size_t k, size_t begin, size_t end);
}

#endif