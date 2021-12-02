#include <cmath>

#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include <tapa.h>

#include "bfs.h"
#include "nxgraph.hpp"

using std::clog;
using std::endl;
using std::runtime_error;
using std::vector;

using namespace nxgraph;


int main(int argc, char* argv[]) {
  //const size_t num_partitions = argc > 2 ? atoi(argv[2]) : 1;
  const size_t num_partitions = 3;
  pair<vector<Interval*>, vector<Shard*>> partitions =
      nxgraph::LoadEdgeList(argv[1], num_partitions);

  vector<Interval*> intervals = partitions.first;
  vector<Shard*> shards = partitions.second;

  clog << "********************Graph Partition********************" << endl;
  clog << "num of intervals: " << intervals.size() << endl;
  clog << "num of shards: " << shards.size() << endl;

  int counter = 0;
  for(auto interval : intervals) {
    clog << "Inside Interval " << counter << ": " << endl;
    for(auto vertex : interval->vertices) {
      clog << vertex->id << " ";
    }
    clog << "" << endl;
    counter += 1;
  }

  counter = 0;
  int sub_counter = 0;
  for(auto shard : shards) {
    clog << "Inside Shard " << counter << ": " << endl;
    for(auto& subshard : shard->subshards) {
      clog << "  Inside SubShard " << sub_counter << counter << ": " << endl;
      for(auto edge : subshard->edges) {
        clog << "    " << edge->src->id << " -> " << edge->dst->id << endl;
      }
      sub_counter += 1;
    }
    sub_counter = 0;
    counter += 1;
  }
  clog << "********************Graph Partition********************" << endl;
  

  return 0;
}
