#include <cassert>
#include <random>
#include <algorithm>
#include <vector>
#include "random_factory.h"

namespace random_factory{
  using namespace std;

  engine eng;

  void seed(result_type val){
    eng.seed(val);
  }

  result_type gen(result_type min, result_type max){
    distribution dist(min, max);
    return dist(eng);
  }

  vector<size_t> k_random_nonrepeat(size_t k, size_t begin, size_t end){ // [begin, end)
    assert(end - begin >= k && "Not enough nodes to find K_NODES different nodes");
    vector<size_t> v(end-begin);
    generate(v.begin(), v.end(), [x = begin] () mutable {return x++;});
    shuffle(v.begin(), v.end(), eng);
    v.resize(k);
    return v;
  }

  vector<size_t> k_random(size_t k, size_t begin, size_t end){
    vector<size_t> v(k);
    distribution distr(begin, end-1);
    for(auto &x : v) x = distr(eng);
    return v;
  }

}