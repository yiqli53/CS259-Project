#include <cstdint>
#include <tapa.h>
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


// TAPA
void Mmap2Stream_Interval(
     tapa::mmap<Interval*> mmap_Interval, 
     uint64_t P,
     tapa::ostream<Interval*>& stream_Interval) 
     {
       for(uint64_t i = 0; i < P; ++i) 
         {
           stream_Interval.write(mmap_Interval[i]);
         }
     }
void  Mmap2Stream_Shard(
      tapa::mmap<Shard*> mmap_Shard,
      uint64_t P,
      tapa::ostream<Shard*>& stream_Interval)
      {
        for(uint64_t i = 0; i < P; ++i)
         {
          stream_Interval.write(mmap_Shard[i]);
         }
      }

void  BFS_k(
      tapa::istream<Interval*>& intervals,
      tapa::istream<Shard*>& shards,
      tapa::ostream<Interval*>& updated_intervals,
      uint64_t P)
      {  
         Bfs(intervals, shards, updated_intervals, P); 
      }


void  Stream2Mmap_Updated_Interval(
      tapa::istream<Interval*>& stream_Updated_Interval,
      tapa::mmap<Interval*> mmap_Updated_Interval,
      uint64_t P)
      {
        for(uint64_t i = 0; i < P; ++i) 
        {
          mmap_Updated_Interval[i] = stream_Updated_Interval.read();
        }
      }

void WriteResult(vector<Vertex*> vertices) 
      { 
      ofstream MyFile("../answer.txt"); 
      for(auto& vertex : vertices) 
      if(vertex->id >= 0) MyFile << vertex->id << ":" << vertex->depth << endl; 
      MyFile.close(); 
      } 

int  main(
      int argc, 
      char* argv[])
      {
       const uint64_t num_partitions = 1000;
       // Partition
       Result* result = PartitionGraph(argv[1], num_partitions);
       vector<Interval*> intervals = result->intervals;
       vector<Shard*> shards = result->shards;
       vector<Vertex*> vertices = result->vertices;
       vector<Edge*> edges = result->edges;

       // Store to MMAP
       tapa::mmap<Interval*> Interval_mmap (&intervals[0]);
       tapa::mmap<Shard*> Shard_mmap (&shards[0]);
       tapa::mmap<Interval*> Updated_Interval_mmap (&intervals[0]);


       //
       tapa::stream<Interval*, num_partitions> Intervals_q("Interval_mmap");
       tapa::stream<Shard*, num_partitions> Shards_q("Shard_mmap");
       tapa::stream<Interval*, num_partitions> Updated_Intervals_q("Updated_Interval_mmap");

       tapa::task()
         .invoke(Mmap2Stream_Interval, Interval_mmap, num_partitions, Intervals_q)
         .invoke(Mmap2Stream_Shard, Shard_mmap, num_partitions, Shards_q)
         .invoke(BFS_k, Intervals_q, Shards_q, Updated_Intervals_q, num_partitions)
         .invoke(Stream2Mmap_Updated_Interval, Updated_Intervals_q, Updated_Interval_mmap, num_partitions);
       
       WriteResult(vertices);
       return 0;  
      }