#include <iostream> // std::cout
#include <string> // std::string
#include <algorithm> // std::sort
#include <cmath> // std::floor
#include "spp.h"
#include "spp_queues.h"
#include "graph.h"
#include "benchmark.h" 

namespace benchmark{
  using namespace std;

  Benchmark::Benchmark()
    : r_cur(graph::NIL_NODE){}

  Benchmark::Benchmark(const char *fileName)
    : g(fileName), sp(g.n_nodes()),
      dh(g.n_nodes(), g.n_nodes()),
      rh(g.n_nodes()),
      r_cur(graph::NIL_NODE){}
  
  
  //source,new_source,group,dijkstra_sheap,r-dijktra_sheap,disjktra_radix_heap,r-dijkstra_radix_heap
  void Benchmark::run_r(spp::node_t r){
    if(r_cur != r) set_r(r_cur = r);


    vector<pair<spp::dist_t, spp::node_t>> nodesByDist;
    nodesByDist.reserve(g.max_node() - g.min_node() + 1);
    for(spp::node_t u = g.min_node(); u <= g.max_node(); ++u)
      nodesByDist.emplace_back(r_ans.first[u], u);
    sort(nodesByDist.begin(), nodesByDist.end());

    // cout<<"Nodes: "<<g.n_nodes()<<endl;
    // cout<<"Arcs: "<<g.n_arcs()<<endl;
    // cout<<"Min Node: "<<g.min_node()<<endl;
    // cout<<"Max Node: "<<g.max_node()<<endl;

    // for(int i=g.min_node();i<=g.max_node();++i){
    //   cout<<i<<": ";
    //   for(auto x : g[i])
    //     cout<<"("<<x.first<<" "<<x.second<<") ";
    //   cout<<endl;
    // }
    // cout<<"SOURCE(R) "<<r<<": ";
    // for(auto x : nodesByDist)
    //   cout<<"("<<x.first<<", "<<x.second<<") ";
    // cout<<"\n";

    //todo...

    size_t group_size = nodesByDist.size()/J_GROUPS; //floor
    size_t ceiled_groups = nodesByDist.size()%J_GROUPS;

    //ceiled portion (first nodesByDist.size()%J_GROUPS groups)
    for(size_t j=0;j<ceiled_groups;++j){
      size_t l = j*(group_size+1), r = l + group_size;
      pair<size_t, size_t> range = find_k_s(nodesByDist, l, r);
      //process the new sources s...
    }

    //floor portion
    for(size_t j=0;j<J_GROUPS-ceiled_groups;++j){
      size_t l = ceiled_groups*(group_size+1) + j*group_size, r = l + group_size - 1;
      pair<size_t, size_t> range = find_k_s(nodesByDist, l, r);
      //process the new sources s...
    }
  }

  
  //==============PRIVATE==============

  void Benchmark::set_r(spp::node_t r){
    reset(r_ans, rh);
    sp.dijkstra(g, r, r_ans, rh);
    r_spp_tree = sp.spp_tree(r_ans.second);
  }

  pair<size_t, size_t> Benchmark::find_k_s(const vpdn &nodesByDist, size_t l, size_t r){
    assert(r - l + 1 >= K_NODES && "Not enough nodes to find K_NODES");

    double avg = 0;
    size_t t = 0;
    for(size_t i=l;i<=r && nodesByDist[i].first!=spp::INF;++i)
      avg += (nodesByDist[i].first - avg)/(++t);
    
    if(t < K_NODES) return {l, l + K_NODES - 1}; //not enough "reachable" nodes

    //get the K_NODES closest to the average
    long long ans_l, ans_r;
    ans_r = upper_bound(nodesByDist.begin()+l, nodesByDist.begin()+l+t, pdn(floor(avg),0)) - nodesByDist.begin();
    ans_l = ans_r - 1;

    while(ans_r - ans_l - 1 < K_NODES){
      if(ans_l<l) ans_l=l-1, ans_r = l + K_NODES;
      else if(ans_r>=r) ans_r=l+t, ans_l = ans_r - K_NODES - 1;
      else if(abs(nodesByDist[ans_l].first - avg) < abs(nodesByDist[ans_r].first - avg)) --ans_l;
      else ++ans_r;
    }

    return {ans_l+1, ans_r-1};
  }

}
