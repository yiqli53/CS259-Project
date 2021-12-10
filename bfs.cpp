
#include <cassert>
#include <cstring>

#include <iostream>

#include <tapa.h>

#include "bfs.h"

using std::ostream;
using std::clog;


void BFS(vector<Interval*> intervals, vector<Shard*> shards, vector<Vertex*> vertices, vector<Edge*> edges, int num_partitions) {
  vector<Vertex*> read_cache;
  vector<Vertex*> write_cache;

  int i = 0;
  int total = edges.size()/100;
  int counter = 0;
  int percent = 0;
  for(auto& current_shard : shards) {
    
    write_cache = intervals[i]->vertices;
    map<int, int> write_map;
    for(int ii = 0 ; ii < write_cache.size() ; ii++) {
      write_map.insert(pair<int, int>(write_cache[ii]->id, ii));
    }

    for(int j = 0 ; j < num_partitions ; j++) {
      read_cache = intervals[j]->vertices;
      map<int, int> read_map;
      for(int jj = 0 ; jj < read_cache.size() ; jj++) {
        read_map.insert(pair<int, int>(read_cache[jj]->id, jj));
      }

      for(auto& edge : current_shard->subshards[j]->edges) {
        int parent_id = edge->src->id;
        int parent_level = edge->src->depth;
        
        int children_id = edge->dst->id;
        int children_level = edge->dst->depth;

        //if(children_id==2) clog << "2 has parent " << parent_id << " with depth " << parent_level << endl;

        if((children_level > parent_level+1 || children_level == -1) && (parent_level != -1)) {
          edge->dst->depth = parent_level+1;
        }

        if(counter == total) {
          cout << ++percent << "%" << endl;
          counter = 0;
        }
        counter += 1;
      }

      
    }

    i += 1;
  }
}