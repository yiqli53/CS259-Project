
#include <cassert>
#include <cstring>

#include <iostream>

#include <tapa.h>

#include "bfs.h"

using std::ostream;
using std::clog;

//make a local copy
vector<Vertex> CopyToCache(Interval* original) {
  vector<Vertex> copied;
  vector<Vertex*> original_vertices = original->vertices;
  for(auto& vertex : original_vertices) {
    Vertex new_vertex = *vertex;
    copied.push_back(new_vertex);
  }
  return copied;
}

int FindVertexById(vector<Vertex> vertices, const int id) {
  for(int i = 0 ; i < vertices.size() ; i++) {
    if(vertices[i].id == id) return i;
  }
  return -1;
}

void BFS(vector<Interval*> intervals, vector<Shard*> shards, vector<Vertex*> vertices, vector<Edge*> edges, int num_partitions) {
  vector<Vertex> read_cache;
  vector<Vertex> write_cache;

  int i = 0;
  int total = edges.size()/100;
  int counter = 0;
  int percent = 0;
  for(auto& current_shard : shards) {
    write_cache = CopyToCache(intervals[i]);
    for(int j = 0 ; j < num_partitions ; j++) {
      read_cache = CopyToCache(intervals[j]);
      for(auto& edge : current_shard->subshards[j]->edges) {
        int parent_id = edge->src->id;
        int parent_level = read_cache[FindVertexById(read_cache, parent_id)].depth;
        if(i == j) {
          int temp = write_cache[FindVertexById(write_cache, parent_id)].depth;
          if((parent_level > temp && temp != -1) || (parent_level==-1 && temp != -1))
            parent_level =  temp;
        }
        
        
        int children_id = edge->dst->id;
        int children_level = write_cache[FindVertexById(write_cache, children_id)].depth;
        
        if((children_level > parent_level+1 && parent_level != -1)|| (children_level == -1 && parent_level != -1)) {
          int target = FindVertexById(write_cache, children_id);
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