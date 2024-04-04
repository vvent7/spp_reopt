#include <random>
#include "rand.h"

namespace rnd{
  Rand::Rand() {}
  Rand::Rand(result_type seed) : eng(seed) {}
  Rand::Rand(result_type a, result_type b) : distr(a, b) {}
  Rand::Rand(result_type seed, result_type a, result_type b) : eng(seed), distr(a, b) {}

  result_type Rand::operator()(){
    return distr(eng);
  }

  result_type Rand::min() const{
    return distr.min();
  }
  result_type Rand::max() const{
    return distr.max();
  }
}