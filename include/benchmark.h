#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

// #include <vector> //std::vector
#include <string> // std::string
#include "spp.h"
#include "graph.h"
#include "spp_queues.h"

namespace benchmark{

  constexpr int NREPS_WARMUP = 2;
  constexpr int NREPS = 10;
  constexpr int J_GROUPS = 10;
  constexpr int K_NODES = 10;

  typedef enum {DIJKSTRA, RDIJKSTRA} SppAlgo;
  typedef enum {DHEAP, RADIX} SppQueue;

  using pdn = std::pair<spp::dist_t, spp::node_t>;
  using vpdn = std::vector<std::pair<spp::dist_t, spp::node_t>>;

  class Benchmark {
  public:
    Benchmark();
    Benchmark(const char *fileName);
    Benchmark(const std::string &fileName);

    void run_r(spp::node_t r);

  private:
    graph::GraphW g;
    spp::SPP sp;

    //queues
    spp::dheap_spp dh;
    spp::radix_heap_spp rh;

    //answers (buffers to reuse)
      //First root (r)
    spp::node_t r_cur;
    spp::answer_t r_ans;
    graph::Graph r_spp_tree;
      //Second root (s)
    spp::answer_t s_ans;

    template<typename queue_t>
    void reset(spp::answer_t &ans, queue_t &q){
      auto &[dist, p] = ans;
      dist.assign(g.n_nodes() + 1, spp::INF);
      p.assign(g.n_nodes() + 1, 0);
      q.clear();
    }

    void set_r(spp::node_t r);
    
    std::pair<size_t, size_t> find_k_s(const vpdn &nodesByDist, size_t l, size_t r);
  };
}


#endif