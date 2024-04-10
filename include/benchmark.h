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
  constexpr int K_NODES = 5;     // number of nodes by group
  constexpr int NREPS_WARMUP = 0; // reps to warmup (by new source S)
  constexpr int NREPS = 1;        // reps to measure (by new source S)

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
    spp::distances_t r_dist;
    spp::parents_t r_parents;
    graph::Graph r_spp_tree;
      //Second root (s)
    spp::distances_t s_dist;

    template<typename queue_t>
    void reset(queue_t &q, spp::distances_t &dist){
      q.clear();
      dist.assign(g.n_nodes() + 1, spp::INF);
    }
    template<typename queue_t>
    void reset(queue_t &q, spp::distances_t &dist, spp::parents_t &parents){
      reset(q, dist);
      parents.resize(g.n_nodes() + 1);
      for(auto &v : parents) v.clear();
    }
    

    void set_r(spp::node_t r);

    std::vector<double> run_s(spp::node_t s);

    std::pair<size_t, size_t> find_k_s_avg(size_t begin, size_t end, const vpdn &nodesByDist);
  };
}


#endif