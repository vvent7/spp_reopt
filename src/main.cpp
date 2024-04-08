#include <bits/stdc++.h>
#include <random>
#include "rand.h"
#include "graph.h"
#include "spp.h"

using namespace std;

constexpr int P = 10; //number of sources (r)
constexpr int K = 10; //number or groups (s groups)
constexpr int L = 10; //number of nodes per group (s)

int main(int argc, char *argv[]){
  if(argc != 3){
    cerr<<"Usage: "<<argv[0]<<" <graph_file> <seed>"<<endl;
    return 1;
  }

  string filePath(argv[1]);
  rnd::result_type seed = stoull(argv[2]);
  
  graph::GraphW g(filePath);
  rnd::Rand rng(seed, g.min_node(), g.max_node());
  
  spp::init(g);
  spp::answer_t &r_ans = spp::ans1, &s_ans = spp::ans2;

  for(int p=1;p<=P;++p){
    spp::node_t r = rng();

    

  }

  return 0;
}