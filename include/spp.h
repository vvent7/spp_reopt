#ifndef _SPP_H_
#define _SPP_H_

#include <utility> // std::pair
#include <climits> // ulonglong
#include <vector> // std::vector
#include <limits> // std::numeric_limits
#include "graph.h" // types, graph::Graph
#include "timer.h" // timer

namespace spp{
  using namespace graph;
  using dist_t = unsigned long long;
  using distances_t = std::vector<dist_t>;
  using parents_t = std::vector<std::vector<node_t>>;
  constexpr dist_t INF = std::numeric_limits<dist_t>::max();
  
  extern double lastTiming;

  Graph spp_tree(parents_t &p);

  template<typename queue_t>
  void dijkstra_parents(const GraphW &g, node_t s, distances_t &d, parents_t &p, queue_t &q){
    q.clear();
    d.assign(g.n_nodes() + 1, spp::INF);
    p.resize(g.n_nodes() + 1);
    for(auto &v : p) v.clear();

    { //timing scope
      timer t(&lastTiming);

      q.insert(d[s]=0, s);
      p[s].resize(1), p[s][0] = s; //root
      while(!q.empty()){
        auto [du, u] = q.extract_min();

        for(auto [v, w] : g[u]){
          auto d2 = du + w;
          if(d2 < d[v]){
            p[v].resize(1), p[v][0] = u;
            if(d[v]==INF) q.insert(d[v] = du+w, v);
            else q.decrease_key(d[v] = du+w, v);
          }
          if(d2 == d[v]) p[v].push_back(u);
        }
      }
    }
  }

  template<typename queue_t>
  void dijkstra(const GraphW &g, node_t s, distances_t &d, queue_t &q){
    q.clear();
    d.assign(g.n_nodes() + 1, spp::INF);

    { //timing scope
      timer t(&lastTiming);

      q.insert(d[s]=0, s); //root
      while(!q.empty()){
        auto [du, u] = q.extract_min();
        for(auto [v, w] : g[u]){
          if(d[v] > du + w){
            if(d[v]==INF) q.insert(d[v] = du+w, v);
            else q.decrease_key(d[v] = du+w, v);
          }
        }
      }
    }
  }

  template<typename queue_t> // r is previous source
  void r_dijkstra(const GraphW &g, node_t s, distances_t &ds, const distances_t &dr, const Graph &r_tree, queue_t &q){
    static std::vector<node_t> done;

    q.clear();
    ds.assign(g.n_nodes() + 1, spp::INF);
    if(done.size() < g.n_nodes() + 1) done.resize(g.n_nodes() + 1);

    { //timing scope
      timer t(&lastTiming);

      //reoptimization
      size_t doneSz=1;
      done[0] = s; ds[s] = 0; //root
      for(size_t i=0;i<doneSz;++i){
        for(auto v : r_tree[done[i]]){
          if(ds[v]!=INF) continue; //already done (reoptimization)
          ds[v] = dr[v] - dr[s];
          done[doneSz++] = v;
        }
      }

      // q.build(ds, toQueue, toQueue + toQueueSz);
      q.build(ds, done.begin(), done.begin() + doneSz);
      //==============

      while(!q.empty()){
        auto [du, u] = q.extract_min();
        for(auto [v, w] : g[u]){
          if(ds[v] > du + w){
            if(ds[v]==INF) q.insert(ds[v] = du+w, v);
            else q.decrease_key(ds[v] = du+w, v);
          }
        }
      }
    }
  }

}

#endif