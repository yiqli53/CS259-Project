
#include <cassert>
#include <cstring>

#include <iostream>

#include <tapa.h>

#include "bfs.h"

using std::ostream;

void BFS(vector<Interval*> intervals, vector<Shard*> shards, const int num_partitions) {
  bool has_update = true;
  while(has_update) {
    has_update = false;
    for(auto& current_shard : shards) {
      for(int i = 0 ; i < num_partitions ; i++) {
        for(auto& edge : current_shard->subshards[i]->edges) {
          int parent_id = edge->src->id;
          int parent_level = edge->src->depth;

          int children_id = edge->dst->id;
          int children_level = edge->dst->depth;

          if((children_level > parent_level+1 || children_level == -1) && (parent_level > -1)) {
            edge->dst->depth = parent_level+1;
            has_update = true;
          }
        }
      }
    }
  }
}