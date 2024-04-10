#include "graph.h"
#include <utility> // std::pair
#include <vector> // std::vector
#include <string> // std::string
#include <algorithm> // std::min, std::max
#include <limits> // std::numeric_limits
#include <fstream> // std::ifstream
#include <iostream>
#include <assert.h>
#include <cstring>

namespace graph{

  // Graph
  Graph::Graph()
    : n(0), m(0),
      miNd(std::numeric_limits<node_t>::min()),
      mxNd(std::numeric_limits<node_t>::max()) { }
  Graph::Graph(node_t n)
    : n(n), m(0), adj(n + 1),
      miNd(std::numeric_limits<node_t>::min()),
      mxNd(std::numeric_limits<node_t>::max()) { }
 

  node_t Graph::n_nodes() const { return n; }
  arc_t Graph::n_arcs() const { return m; }
  node_t Graph::min_node() const { return miNd; }
  node_t Graph::max_node() const { return mxNd; }
  
  const std::vector<node_t>& Graph::operator[](node_t u) const { return adj[u]; }

  void Graph::add_arc(node_t u, node_t v){
    adj[u].emplace_back(v);
    ++m;
    miNd = std::min({miNd, u, v});
    mxNd = std::max({mxNd, u, v});
  }

  // GraphW
  GraphW::GraphW()
    : n(0), m(0) { init(); }
  GraphW::GraphW(node_t n)
    : n(n), m(0), adj(n + 1) { init(); }
  GraphW::GraphW(const char *fileName) : m(0) {
    init();

    const char *pPoint = strrchr(fileName, '.');

    if(strcmp(pPoint, ".txt") == 0){
      std::ifstream f(fileName);
      
      assert(f.is_open() && "Instance File not found");

      arc_t auxM;

      f>>n>>auxM; //nodes arcs
      for(node_t i=0;i<=n;++i) //jumping N lines (nd 100)
        f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      adj.resize(n + 1);
      for(arc_t i=0;i<auxM;++i){
        node_t u, v;
        weight_t w;
        f>>u>>v>>w;
        add_arc(u, v, w);

      }
    }
    else if(strcmp(pPoint, ".gr") == 0){ //DIMACS
      std::ifstream f(fileName);
      
      assert(f.is_open() && "Instance File not found");

      std::string line;
      while(getline(f, line)){
        if(line[0] == 'p'){
          arc_t auxM;
          sscanf(line.c_str(), "%*s %*s %u %u", &n, &auxM);
          adj.resize(n + 1);
        }
        else if(line[0] == 'a'){
          node_t u, v;
          weight_t w;
          sscanf(line.c_str(), "%*s %u %u %llu", &u, &v, &w);
          add_arc(u, v, w);
        }
      }
    }
  }
  GraphW::GraphW(const std::string &fileName) : GraphW(fileName.c_str()) { }

  void GraphW::init(){
    miNd = std::numeric_limits<node_t>::max();
    mxNd = std::numeric_limits<node_t>::min();
    miW = std::numeric_limits<weight_t>::max();
    mxW = std::numeric_limits<weight_t>::min();
  }

  node_t GraphW::n_nodes() const { return n; }
  arc_t GraphW::n_arcs() const { return m; }
  weight_t GraphW::min_weight() const { return miW; }
  weight_t GraphW::max_weight() const { return mxW; }
  node_t GraphW::min_node() const { return miNd; }
  node_t GraphW::max_node() const { return mxNd; }

  const std::vector<pnw>& GraphW::operator[](node_t u) const { return adj[u]; }

  void GraphW::add_arc(node_t u, node_t v, weight_t w){
    adj[u].emplace_back(v, w);
    miW = std::min(miW, w);
    mxW = std::max(mxW, w);
    miNd = std::min({miNd, u, v});
    mxNd = std::max({mxNd, u, v});
    ++m;
  }

}