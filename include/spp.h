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
  using answer_t = std::pair<std::vector<dist_t>, std::vector<node_t>>; //{dists, parents}
  constexpr dist_t INF = std::numeric_limits<dist_t>::max();

  class SPP{
  public:
    SPP() : n(0), buffer1(nullptr), buffer2(nullptr) { }
    SPP(const node_t n)
      : n(n), buffer1(new node_t[n+1]), buffer2(new node_t[n+1]) { }
    SPP(const GraphW &g) : SPP(g.n_nodes()){ }

    ~SPP(){
      delete [] buffer1;
      delete [] buffer2;
    }
    
    Graph spp_tree(std::vector<node_t> &p){
      Graph tr(p.size());
      for(node_t i=0;i<(node_t)p.size();++i)
        if(p[i]!=i) tr.add_arc(p[i], i);
      return tr;
    }

    template<typename queue_t>
    void dijkstra(const GraphW &g, node_t s, answer_t &ans, queue_t &q){
      timer t(&lastTiming);

      auto &[d, p] = ans;

      q.insert(d[s]=0, s); p[s] = s; //root
      while(!q.empty()){
        auto [du, u] = q.extract_min();

        for(auto [v, w] : g[u]){
          if(d[v] > du + w){
            p[v]=u;
            if(d[v]==INF) q.insert(d[v] = du+w, v);
            else q.decrease_key(d[v] = du+w, v);
          }
        }
      }
    }

    template<typename queue_t> // r is previous source
    answer_t r_dijkstra(const GraphW &g, node_t s, answer_t &ans, const answer_t &r_ans, const Graph &r_tree, queue_t &q){
      timer t(&lastTiming);

      auto &[dr, pr] = r_ans;
      auto &[ds, ps] = ans;

      //reoptimization
      node_t *done = buffer1, *toQueue = buffer2;
      size_t doneSz=1, toQueueSz=0;

      done[0] = s; ds[s] = 0; ps[s] = s; //root
      for(size_t i=0;i<doneSz;++i){
        for(auto v : r_tree[done[i]]){
          ps[v] = done[i];
          ds[v] = dr[v] - dr[s];
          done[doneSz++] = v;
        }
      }
      
      for(size_t i=0;i<doneSz;++i){
        node_t u = done[i];
        for(auto [v, w] : g[u]){
          if(ds[v] > ds[u] + w){
            if(ds[v]==INF) toQueue[toQueueSz++] = v;
            ds[v] = ds[u] + w;
            ps[v] = u;
          }
        }
      }
      
      q.build(ds, toQueue, toQueue + toQueueSz);
      //==============

      while(!q.empty()){
        auto [du, u] = q.extract_min();

        for(auto [v, w] : g[u]){
          if(ds[v] > du + w){
            ps[v]=u;
            if(ds[v]==INF) q.insert(ds[v] = du+w, v);
            else q.decrease_key(ds[v] = du+w, v);
          }
        }
      }

      return ans;
    }

    double last_timing() const { return lastTiming; }

  private:
    node_t n;
    node_t *buffer1, *buffer2;
    double lastTiming;
  };
}

#endif