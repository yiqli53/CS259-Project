
#include <cassert>
#include <cstring>

#include <iostream>

#include <tapa.h>

#include "bfs.h"

using std::ostream;
using std::clog;

//make a local copy
vector<Vertex> CopyToCache(vector<Vertex*> original_vertices) {
  vector<Vertex> copied;
  for(auto& vertex : original_vertices) {
    Vertex new_vertex = *vertex;
    copied.push_back(new_vertex);
  }
  return copied;
}

void BFS(vector<Interval*> intervals, vector<Shard*> shards, vector<Vertex*> vertices, vector<Edge*> edges, int num_partitions) {
  vector<Vertex> read_cache;
  vector<Vertex> write_cache;

  int i = 0;
  int total = edges.size()/100;
  int counter = 0;
  int percent = 0;
  for(auto& current_shard : shards) {
    write_cache = CopyToCache(intervals[i]->vertices);
    map<int, int> write_map;
    for(int ii = 0 ; ii < write_cache.size() ; ii++) {
      write_map.insert(pair<int, int>(write_cache[ii].id, ii));
    }

    for(int j = 0 ; j < num_partitions ; j++) {
      read_cache = CopyToCache(intervals[j]->vertices);
      map<int, int> read_map;
      for(int jj = 0 ; jj < read_cache.size() ; jj++) {
        read_map.insert(pair<int, int>(read_cache[jj].id, jj));
      }
      for(auto& edge : current_shard->subshards[j]->edges) {
        int parent_id = edge->src->id;
        int parent_level = read_cache[read_map.find(parent_id)->second].depth;
        if(i == j) {
          int temp = write_cache[write_map.find(parent_id)->second].depth;
          if((parent_level > temp && temp != -1) || (parent_level==-1 && temp != -1))
            parent_level =  temp;
        }
        
        int children_id = edge->dst->id;
        int children_level = write_cache[write_map.find(children_id)->second].depth;
        
        if((children_level > parent_level+1 && parent_level != -1)|| (children_level == -1 && parent_level != -1)) {
          int target = write_map.find(children_id)->second;
          if(parent_level != -1 && (write_cache[target].depth > parent_level+1 || write_cache[target].depth == -1)) {
            write_cache[target].depth = parent_level+1;
          }
        }

        if(counter == total) {
          cout << ++percent << "%" << endl;
          counter = 0;
        }
        counter += 1;
      }

      
    }
    
    int temp = 0;
    for(Vertex vertex : write_cache) {
      if(vertex.depth < intervals[i]->vertices[temp]->depth || intervals[i]->vertices[temp]->depth == -1) {
        intervals[i]->vertices[temp]->depth = vertex.depth;
      }
      temp += 1;
    }

    i += 1;
  }
}