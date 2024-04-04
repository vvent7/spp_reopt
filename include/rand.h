#ifndef _RAND_H_
#define _RAND_H_

#include <random>

namespace rnd{
  using engine = std::mt19937_64;
  using result_type = engine::result_type;
  using distribution = std::uniform_int_distribution<result_type>;

  class Rand{ //64 bits random uniform number generator
  public:
    Rand();
    Rand(result_type seed);
    Rand(result_type a, result_type b);
    Rand(result_type seed, result_type a, result_type b);
    result_type operator()();

    result_type min() const;
    result_type max() const;

  private:
    engine eng;
    distribution distr;
  };
}

#endif