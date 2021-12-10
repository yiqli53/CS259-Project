
#include <cstddef>
#include <cstdlib>
#include <map>

#include <algorithm>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <glog/logging.h>

using namespace std;
using std::clog;

namespace nxgraph {

struct Vertex {
  int id;
  int depth;
  int interval;
  Vertex(int ax, int ay, int az) : id(ax), depth(ay), interval(az){}
};

struct Edge {
  Vertex* src;
  Vertex* dst;
};

struct Interval {
  vector<Vertex*> vertices;
};

struct SubShard {
  vector<Edge*> edges;
};

struct Shard {
  vector<SubShard*> subshards;
};

struct Result {
  vector<Vertex*> vertices;
  vector<Edge*> edges;
  vector<Interval*> intervals;
  vector<Shard*> shards;
};

inline pair<vector<Edge*>, vector<Vertex*>> ReadFile(
    const char* begin_ptr, const char* end_ptr, int* max_int = nullptr,
    int* min_int = nullptr) {
  using std::nullptr_t;
  using std::numeric_limits;
  using std::string;
  using std::strtoull;
  using std::unique_ptr;
  using std::vector;
  if (max_int != nullptr) {
    *max_int = numeric_limits<int>::min();
  }
  if (min_int != nullptr) {
    *min_int = numeric_limits<int>::max();
  }


  map<int, int> vertex_map;
  const char* next_pos = nullptr;
  vector<Edge*> edges;
  vector<Vertex*> vertices;
  int count = 0;
  for (const char* ptr = begin_ptr; ptr < end_ptr; ++ptr) {
    // Skip spaces and tabs.
    for (; isblank(*ptr); ++ptr) {
      if (ptr >= end_ptr) {
        break;
      }
    }

    /// Skip comment lines.
    for (; *ptr == '#'; ++ptr) {
      for (; *ptr != '\n' && *ptr != '\r'; ++ptr) {
        if (ptr >= end_ptr) {
          break;
        }
      }
      for (; isblank(*ptr); ++ptr) {
        if (ptr >= end_ptr) {
          break;
        }
      }
      if (ptr >= end_ptr) {
        break;
      }
    }


    
    /// Swallows empty lines.
    const int src = strtoull(ptr, const_cast<char**>(&next_pos), 10);
    /*
    if (next_pos == ptr) {
      break;
    }
    */


    /// Skip non-digits.
    for (; !isdigit(*next_pos); ++next_pos) {
      if (next_pos >= end_ptr) {
        break;
      }
    }

    const int dst = strtoull(next_pos, const_cast<char**>(&ptr), 10);
    if (next_pos == ptr) {
      break;
    }

    /// Skip spaces and tabs.
    for (; isblank(*ptr); ++ptr) {
      if (ptr >= end_ptr) {
        break;
      }
    }
    

    //check if src/dst Vertex already exist; if not, create them
    Vertex* src_vertex;
    Vertex* dst_vertex;
    auto src_find = vertex_map.find(src);
    auto dst_find = vertex_map.find(dst);
    if(src_find != vertex_map.end()) {
      src_vertex = vertices[(src_find->second)];
    } else {
      src_vertex = new Vertex(src, -1, -1);
      vertices.push_back(src_vertex);
      vertex_map.insert(std::pair<int, int>(src, count));
      count += 1;
    }

    if(dst_find != vertex_map.end()) {
      dst_vertex = vertices[(dst_find->second)];
    } else {
      dst_vertex = new Vertex(dst, -1, -1);
      vertices.push_back(dst_vertex);
      vertex_map.insert(std::pair<int, int>(dst, count));
      count += 1;
    }


    Edge* edge = new Edge{src_vertex, dst_vertex};
    if (*ptr != '\n' && *ptr != '\r') {
      ptr = next_pos;
    }
    if (max_int != nullptr) {
      *max_int = std::max({*max_int, src, dst});
    }
    if (min_int != nullptr) {
      *min_int = std::min({*min_int, src, dst});
    }
    edges.push_back(edge);
  }

  vertices[0]->depth=0;

  pair<vector<Edge*>, vector<Vertex*>> result;
  result.first = edges;
  result.second = vertices;

  return result;
}

inline Result* PartitionGraph(const string& filename, int num_partitions) {
  
  using std::runtime_error;

  int fd = open(filename.c_str(), O_RDONLY);
  if (fd == -1) {
    throw runtime_error("cannot open file " + filename);
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    throw runtime_error("failed to stat " + filename);
  }

  //size of file
  const size_t mmap_length = sb.st_size;

  auto mmap_ptr = reinterpret_cast<char*>(
      mmap(nullptr, mmap_length, PROT_READ, MAP_SHARED, fd, /*offset=*/0));
  if (mmap_ptr == MAP_FAILED) {
    throw runtime_error("failed to mmap " + filename);
  }

  int max_int;
  int min_int;

  //read edges from file
  pair<vector<Edge*>, vector<Vertex*>> read_result = ReadFile(mmap_ptr, mmap_ptr + mmap_length, &max_int, &min_int);


  vector<Edge*> edges = read_result.first;
  vector<Vertex*> vertices = read_result.second;

  int num_vertices = vertices.size();

  int padding = num_partitions - (num_vertices % num_partitions);
  for(int i = 0 ; i < padding ; i++) {
    Vertex* pad = new Vertex(-1, -1, -1);
    vertices.push_back(pad);
  }

  num_vertices = vertices.size();
  const size_t partition_size = num_vertices / num_partitions;

  //p intervals, p shards and p^2 subshards
  vector<Interval*> intervals(num_partitions);
  vector<Shard*> shards(num_partitions);
  for (auto& interval : intervals) {
    vector<Vertex*> new_vertices;
    interval = new Interval{new_vertices};
  }

  for (auto& shard : shards) {
    vector<SubShard*> subshards(num_partitions);
    for(auto& subshard : subshards) {
      vector<Edge*> edges;
      subshard = new SubShard{edges};
    }
    shard = new Shard{subshards};
  }
  int counter = 0;
  for (auto& vertex : vertices) {
    vertex->interval = (counter/partition_size);
    auto test = intervals[counter/partition_size];
    intervals[counter/partition_size]->vertices.push_back(vertex);
    counter += 1;
  }

  for(auto& edge : edges) {
    int src_interval = (edge->src)->interval;
    int dst_interval = (edge->dst)->interval;
    shards[dst_interval]->subshards[src_interval]->edges.push_back(edge);
  }
  if (munmap(mmap_ptr, mmap_length) != 0) {
    throw runtime_error("failed to munmap " + filename);
  }

  if (close(fd) != 0) {
    throw runtime_error("failed to close " + filename);
  }

  Result* result = new Result{vertices, edges, intervals, shards};

  return result;
}

}  // namespace nxgraph
