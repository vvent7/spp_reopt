#include <bits/stdc++.h>
#include <random>
#include "rand.h"
#include "graph.h"

using namespace std;

int main(int argc, char *argv[]){
  string filePath(argv[1]);
  rnd::result_type seed = stoull(argv[2]);
  rnd::Rand rng(seed);

  graph::GraphW g(filePath);

  return 0;
}