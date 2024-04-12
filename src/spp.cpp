#include <utility> // std::pair
#include <climits> // ulonglong
#include <vector> // std::vector
#include <limits> // std::numeric_limits
#include "graph.h" // types, graph::Graph
#include "timer.h" // timer
#include "spp.h"

namespace spp{

  double lastTiming;

  Graph spp_tree(parents_t &p){
    Graph tr(p.size());
    for(node_t i=0;i<(node_t)p.size();++i)
      for(auto u : p[i])
        if(u!=i) tr.add_arc(u, i);
    return tr;
  }

}