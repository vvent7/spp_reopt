#include <utility> // std::pair
#include <climits> // ulonglong
#include <vector> // std::vector
#include <limits> // std::numeric_limits
#include "graph.h" // types, graph::Graph
#include "timer.h" // timer
#include "spp.h"

namespace spp{

  double lastTiming;

  Graph spp_dag(parents_t &p){
    Graph dag(p.size());
    for(node_t i=0;i<(node_t)p.size();++i)
      for(auto u : p[i])
        if(u!=i) dag.add_arc(u, i);
    return dag;
  }
  Graph spp_tree(parents_t &p){
    Graph tr(p.size());
    for(node_t i=0;i<(node_t)p.size();++i){
      if(p[i].empty()) continue;
      tr.add_arc(p[i][0], i);
    }
    return tr;
  }

  void get_orders_tree(const Graph &tree, const std::vector<bool> &vis, std::vector<node_t> &pos_order, int u) {
    for(int v : tree[u])
      if(!vis[v])
        get_orders_tree(tree, vis, pos_order, v);
    pos_order.push_back(u);
  }

  GraphW spp_fs_auction(const GraphW &g, distances_t &d){
    GraphW fs(g.max_node() + 1);
    for(node_t u = g.min_node(); u <= g.max_node(); u++)
      for(auto [v, w] : g[u])
        fs.add_arc(u, v, w + (d[u] - d[v]));
    return fs;
  }

}