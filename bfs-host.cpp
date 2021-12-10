#include <cmath>

#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

#include <tapa.h>

#include "bfs.h"

using std::clog;
using std::endl;
using std::runtime_error;
using std::vector;

using namespace nxgraph;
using namespace std;

void DisplayPartition(vector<Interval*> intervals, vector<Shard*> shards) {
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
}

void DisplayVertices(vector<Vertex*> vertices) {
  clog << "********************Vertex Information********************" << endl;
  for(auto& vertex : vertices) {
    clog << "Vertex " << vertex->id << " has depth of " << vertex->depth << endl;
  }
  clog << "********************Vertex Information********************" << endl;
}


int main(int argc, char* argv[]) {
  const size_t num_partitions = 10000;
  Result* result = PartitionGraph(argv[1], num_partitions);

  cout << "graph partition finish" << endl;

  vector<Interval*> intervals = result->intervals;
  vector<Shard*> shards = result->shards;
  vector<Vertex*> vertices = result->vertices;
  vector<Edge*> edges = result->edges;

  //DisplayPartition(intervals, shards);

  BFS(intervals, shards, vertices, edges, num_partitions);

  /*
  clog << "Interval 2990: " << endl;
  for(auto& vertex : intervals[2989]->vertices)
    clog << vertex->id << " ";
  clog << endl;

  clog << "Interval 3121: " << endl;
  for(auto& vertex : intervals[3120]->vertices)
    clog << vertex->id << " ";
  clog << endl;

  clog << "subshard 3121 2990:" << endl;
  for(auto& edge : shards[3120]->subshards[2989]->edges)
    clog << edge->src->id << " -> " << edge->dst->id << endl;
  */


  //DisplayVertices(vertices);

  ofstream MyFile("../answer.txt");

  for(auto& vertex : vertices) {
    if(vertex->id >= 0) MyFile << vertex->id << ":" << vertex->depth << endl;
  }

  MyFile.close();


  return 0;
}
