#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <utility> // std::pair
#include <vector> // std::vector
#include <string> // std::string
#include <algorithm> // std::min, std::max
#include <limits>

namespace graph{
  using node_t = unsigned int;
  using arc_t = unsigned int;
  using weight_t = long long;
  using pnw = std::pair<node_t, weight_t>;

  constexpr node_t NIL_NODE = std::numeric_limits<node_t>::max();

  //always growing (delete not allowed)
  class Graph{
    public:
      Graph();
      Graph(node_t n);

      node_t n_nodes() const;
      arc_t n_arcs() const;
      node_t min_node() const;
      node_t max_node() const;
      const std::vector<node_t>& operator[](node_t u) const;

      // u -> v
      void add_arc(node_t u, node_t v);

    private:
      node_t n;
      arc_t m;
      std::vector<std::vector<node_t>> adj;
      node_t miNd, mxNd;
  };

  //always growing (delete not allowed)
  class GraphW{
    public:
      GraphW();
      GraphW(node_t n);
      GraphW(const char *fileName);
      GraphW(const std::string &fileName);

      node_t n_nodes() const;
      arc_t n_arcs() const;
      weight_t min_weight() const;
      weight_t max_weight() const;
      node_t min_node() const;
      node_t max_node() const;

      const std::vector<pnw>& operator[](node_t u) const;

      //u -> v (weight w)
      void add_arc(node_t u, node_t v, weight_t w);

    private:
      node_t n;
      arc_t m;
      std::vector<std::vector<pnw>> adj;
      weight_t miW, mxW;
      node_t miNd, mxNd;

      void init();
  };
}


#endif