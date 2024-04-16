#include <iostream> // std::cout
#include <string> // std::string
#include <algorithm> // std::sort
#include <numeric>
#include <cmath> // std::floor
#include <random>
#include "spp.h"
#include "spp_queues.h"
#include "graph.h"
#include "benchmark.h"
#include "random_factory.h"

#define SIZEOF(arr) (sizeof(arr) / sizeof(*arr))

namespace benchmark{
  using namespace std;

  constexpr const char *CSV_HEADER[] = {"source","new_source","group","dijkstra_dheap","rdijkstra_dag_dheap","rdijkstra_tree_dheap","rauction_dag_dheap","rauction_tree_dheap","dijkstra_radix","rdijkstra_dag_radix","rdijkstra_tree_radix","rauction_dag_radix","rauction_tree_radix"};

  Benchmark::Benchmark()
    : r_cur(graph::NIL_NODE){}

  Benchmark::Benchmark(const char *fileName)
    : g(fileName),
      dh(g.n_nodes(), g.n_nodes()),
      rh(g.n_nodes()),
      r_cur(graph::NIL_NODE){}
  
  Benchmark::Benchmark(const std::string &fileName)
    :Benchmark(fileName.c_str()){}
  
  void Benchmark::run_r(spp::node_t r){
    if(r_cur != r) set_r(r_cur = r);

    vector<pair<spp::dist_t, spp::node_t>> nodesByDist(g.max_node() - g.min_node() + 1);
    // nodesByDist.reserve(g.max_node() - g.min_node() + 1);
    for(spp::node_t u = g.min_node(); u <= g.max_node(); ++u){
      nodesByDist[u - g.min_node()] = {r_dist[u], u};
      // nodesByDist.emplace_back(r_dist[u], u);
    }
    sort(nodesByDist.begin(), nodesByDist.end());

    size_t group_size = nodesByDist.size()/J_GROUPS; //floor
    size_t ceiled_groups = nodesByDist.size()%J_GROUPS;

    //will work for both ceiled and floor portions
    for(size_t j=0, begin, end;j<J_GROUPS;++j){
      if(j < ceiled_groups){
        begin = j*(group_size+1);
        end = begin + group_size + 1;
      }
      else{
        // begin = ceiled_groups*(group_size+1) + (j-ceiled_groups)*group_size;
        begin = ceiled_groups + j * group_size;
        end = begin + group_size;
      }

      auto new_sources = random_factory::k_random_nonrepeat(K_NODES, begin, end);
      
      for(auto sI : new_sources){
        spp::node_t s = nodesByDist[sI].second;
        for(int i=0;i<NREPS_WARMUP;++i) run_s(s); //ignore for warmup
        for(int i=0;i<NREPS;++i){
          auto times = run_s(s);
          printf("%u,%u,%zu", r, s, j+1);
          for(auto x : times) printf(",%.3lf", x);
          puts("");
        }
      }

    }
  }

  void Benchmark::run(){
    printf("%s", CSV_HEADER[0]);
    for(int i=1;i<(int)SIZEOF(CSV_HEADER);++i)
      printf(",%s", CSV_HEADER[i]);
    puts("");
    auto r = random_factory::k_random_nonrepeat(P_SOURCES, g.min_node(), g.max_node());
    for(auto rI : r) run_r(rI);
  }

  
  //==============PRIVATE==============

  void Benchmark::set_r(spp::node_t r){
    spp::dijkstra_parents(g, r, r_dist, r_parents, rh);
    r_spp_tree = spp::spp_tree(r_parents);
    r_spp_dag = spp::spp_dag(r_parents);
    r_fs = spp::spp_fs_auction(g, r_dist);
  }

  vector<double> Benchmark::run_s(spp::node_t s){
    vector<double> times(10);
    size_t i=0;

    //================DHEAP================
    //DIJKSTRA, DHEAP
    spp::dijkstra(g, s, s_dist, dh);
    times[i++] = spp::lastTiming;
    
    #ifndef NDEBUG
      auto aux=s_dist;
      #define CHECK_DIST(MSG) if(aux!=s_dist) cout<<"ERROR - "<<MSG<<endl, exit(1);
    #else
      #define CHECK_DIST(MSG)
    #endif

    //RDIJKSTRA-DAG, DHEAP
    spp::r_dijkstra_dag(g, s, s_dist, r_dist, r_spp_dag, dh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("RDIJKSTRA-DAG DHEAP");

    //RDIJKSTRA-TREE, DHEAP
    spp::r_dijkstra_tree(g, s, s_dist, r_dist, r_spp_tree, dh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("RDIJKSTRA-TREE DHEAP");

    //RAUCTION-DAG, DHEAP
    spp::r_auction_dag(r_fs, s, s_dist, r_dist, r_spp_dag, dh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("RAUCTION-DAG DHEAP");
    
    //RAUCTION-TREE, DHEAP
    spp::r_auction_tree(r_fs, s, s_dist, r_dist, r_spp_tree, dh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("RAUCTION-TREE DHEAP");

    //================RADIX================

    //DIJKSTRA, RADIX
    spp::dijkstra(g, s, s_dist, rh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("DIJKSTRA RADIX");

    //RDIJKSTRA-DAG, RADIX
    spp::r_dijkstra_dag(g, s, s_dist, r_dist, r_spp_dag, rh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("RDIJKSTRA-DAG RADIX");

    //RDIJKSTRA-TREE, RADIX
    spp::r_dijkstra_tree(g, s, s_dist, r_dist, r_spp_tree, rh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("RDIJKSTRA-TREE RADIX");

    //RAUCTION-DAG, RADIX
    spp::r_auction_dag(r_fs, s, s_dist, r_dist, r_spp_dag, rh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("RAUCTION-DAG RADIX");

    //RAUCTION-TREE, RADIX
    spp::r_auction_tree(r_fs, s, s_dist, r_dist, r_spp_tree, rh);
    times[i++] = spp::lastTiming;
    CHECK_DIST("RAUCTION-TREE RADIX");

    return times;
  }

  pair<size_t, size_t> Benchmark::find_k_s_avg(size_t begin, size_t end, const vpdn &nodesByDist){
    assert(end - begin >= K_NODES && "Not enough nodes to find K_NODES");

    double avg = 0;
    size_t t = 0;
    for(size_t i=begin;i<end && nodesByDist[i].first!=spp::INF;++i)
      avg += (nodesByDist[i].first - avg)/(++t);
    
    if(t < K_NODES) return {begin, begin + K_NODES}; //not enough "reachable" nodes

    end = begin + t;

    //get the K_NODES closest to the average [begin,end)
    size_t r = upper_bound(nodesByDist.begin()+begin, nodesByDist.begin()+end, pdn(floor(avg),spp::INF)) - nodesByDist.begin();
  
    if(r == begin) return {begin, begin + K_NODES};
    else if(r == end) return {end - K_NODES, end};

    size_t l = r - 1;

    while(r - l - 1 < K_NODES){
      if((avg - nodesByDist[l].first) <= abs(nodesByDist[r].first - avg)){
        if(l != begin) --l;
        else return {begin, begin + K_NODES};
      }
      else if(r+1 != end) ++r;
      else return {end - K_NODES, end};
    }

    return {l+1, r};
  }

}
