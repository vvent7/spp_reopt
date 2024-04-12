#include <bits/stdc++.h>
#include "random_factory.h"
#include "benchmark.h"

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define endl '\n'

using namespace std;

int main(int argc, char *argv[]){_
  if(argc != 3){
    cerr<<"Usage: "<<argv[0]<<" <graph_file> <seed>"<<endl;
    return 1;
  }

  random_factory::seed(stoull(argv[2]));
  benchmark::Benchmark b(argv[1]);
  b.run();

  return 0;
}