#include <bits/stdc++.h>
#include <random>
#include "rand.h"
#include "graph.h"
#include "spp.h"
#include "benchmark.h"

using namespace std;

constexpr int P = 10; //number of sources (r)

int main(int argc, char *argv[]){
  if(argc != 3){
    cerr<<"Usage: "<<argv[0]<<" <graph_file> <seed>"<<endl;
    return 1;
  }

  benchmark::Benchmark b(argv[1]);

  b.run_r(3); //run for root=3
  b.run_r(1); //run for root=4

  return 0;
}