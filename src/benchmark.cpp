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
namespace benchmark{
  using namespace std;

  constexpr auto CSV_HEADER = "source,new_source,group,dijkstra_dheap,dijkstra_radix,rdijkstra_dheap,rdijkstra_radix";

  Benchmark::Benchmark()
    : r_cur(graph::NIL_NODE){}

  Benchmark::Benchmark(const char *fileName)
    : g(fileName), sp(g.n_nodes()),
      dh(g.n_nodes(), g.n_nodes()),
      rh(g.n_nodes()),
      r_cur(graph::NIL_NODE){}
  
  Benchmark::Benchmark(const std::string &fileName)
    :Benchmark(fileName.c_str()){}
  
  void Benchmark::run_r(spp::node_t r){
    if(r_cur != r) set_r(r_cur = r);

    vector<pair<spp::dist_t, spp::node_t>> nodesByDist;
    nodesByDist.reserve(g.max_node() - g.min_node() + 1);
    for(spp::node_t u = g.min_node(); u <= g.max_node(); ++u)
      nodesByDist.emplace_back(r_ans.first[u], u);
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
          cout<<r<<","<<s<<","<<j+1;
          for(auto x : times) cout<<","<<x;
          cout<<endl;
        }
      }

    }
  }

  void Benchmark::run(){
    cout<<CSV_HEADER<<endl;
    auto r = random_factory::k_random_nonrepeat(P_SOURCES, g.min_node(), g.max_node());
    for(auto rI : r) run_r(rI);
  }

  
  //==============PRIVATE==============

  void Benchmark::set_r(spp::node_t r){
    reset(r_ans, rh);
    sp.dijkstra(g, r, r_ans, rh);
    r_spp_tree = sp.spp_tree(r_ans.second);
  }

  vector<double> Benchmark::run_s(spp::node_t s){
    vector<double> times(4);

    //DIJKSTRA, DHEAP
    reset(s_ans, dh);
    sp.dijkstra(g, s, s_ans, dh);
    times[0] = sp.last_timing();
    
    #ifndef NDEBUG
    auto aux=s_ans;
    #endif

    //DIJKSTRA, RADIX
    reset(s_ans, rh);
    sp.dijkstra(g, s, s_ans, rh);
    times[1] = sp.last_timing();

    #ifndef NDEBUG
    if(aux.first!=s_ans.first) cout<<"ERROR - DIJKSTRA RADIX\n", exit(1);
    #endif

    //RDIJKSTRA, DHEAP
    reset(s_ans, dh);
    sp.r_dijkstra(g, s, s_ans, r_ans, r_spp_tree, dh);
    times[2] = sp.last_timing();

    #ifndef NDEBUG
    if(aux.first!=s_ans.first) cout<<"ERROR - RDIJKSTRA DHEAP\n", exit(1);
    #endif

    //RDIJKSTRA, RADIX
    reset(s_ans, rh);
    sp.r_dijkstra(g, s, s_ans, r_ans, r_spp_tree, rh);
    times[3] = sp.last_timing();

    #ifndef NDEBUG
    if(aux.first!=s_ans.first) cout<<"ERROR - RDIJKSTRA RADIX\n", exit(1);
    #endif

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
