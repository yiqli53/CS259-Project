#include <cassert>
#include <cstring>

#include <iostream>

#include <tapa.h>

#include "bfs.h"

using std::ostream;

void Bfs(tapa::istream<Interval*>& intervals,
      tapa::istream<Shard*>& shards,
      tapa::ostream<Interval*>& updated_intervals, uint64_t num_partitions) {   
  
  vector<Interval*> local_intervals;
  vector<Shard*> local_shards;

  for(int i = 0 ; i < num_partitions ; i++) {
    local_shards.push_back(shards.read());
    local_intervals.push_back(intervals.read());
  }
  bool has_update = true;
  while(has_update) {
    has_update = false;
    for(auto& current_shard : local_shards) {
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
  for(int i = 0 ; i < num_partitions ; i++) {
    updated_intervals.write(local_intervals[i]);
  }
}