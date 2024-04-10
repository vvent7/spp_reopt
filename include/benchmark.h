#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

#include <string> // std::string
#include <utility> // std::pair
#include <vector> // std::vector
#include <cstddef> // std::size_t
#include "spp.h"
#include "graph.h"
#include "spp_queues.h"

namespace benchmark{
  constexpr int P_SOURCES = 5;   // number of sources (R)
  constexpr int J_GROUPS = 10;    // number of groups (for each source R)
  constexpr int K_NODES = 10;     // number of nodes by group
  constexpr int NREPS_WARMUP = 1; // reps to warmup (by new source S)
  constexpr int NREPS = 5;        // reps to measure (by new source S)

  using pdn = std::pair<spp::dist_t, spp::node_t>;
  using vpdn = std::vector<std::pair<spp::dist_t, spp::node_t>>;

  class Benchmark {
  public:
    Benchmark();
    Benchmark(const char *fileName);
    Benchmark(const std::string &fileName);

    void run_r(spp::node_t r);
    void run();

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

    std::vector<double> run_s(spp::node_t s);

    std::pair<size_t, size_t> find_k_s_avg(size_t begin, size_t end, const vpdn &nodesByDist);
  };
}


#endif