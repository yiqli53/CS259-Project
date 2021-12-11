#ifndef BFS_H_
#define BFS_H_

#include <cstdint>

#include <ap_int.h>
#include <tapa.h>
#include "nxgraph.hpp"

using namespace nxgraph;

// There is a bug in Vitis HLS preventing fully pipelined read/write of struct
// via m_axi; using ap_uint can work-around this problem.
template <typename T>
using bits = ap_uint<tapa::widthof<T>()>;

void BFS(vector<Interval*> intervals, vector<Shard*> shards, const int num_partitions);

#endif  // BFS_H_
