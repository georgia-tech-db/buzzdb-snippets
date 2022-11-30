
#include <algorithm>
#include <fstream>
#include <iostream>

namespace buzzdb {
namespace utils {

#define UNUSED(p) ((void)(p))



int hash_function(int k, int size){
  return k % size;
}
std::vector<std::pair<int, int>> robin_hood_hashing(std::vector<int> values, int size) {
  std::vector<std::pair<int, int>> storage (size, std::make_pair(-1, -1));
  for (size_t i = 0; i < values.size();i++) {
    int position = hash_function(values[i],size);
    int offset = 0;
    while (storage[position].second >= offset){
      offset += 1;
      position += 1;
    }
    int last_position = position;
    while(storage[last_position].second != -1) {
      last_position += 1;
    }
    for(int j = last_position; j > position; j--) {
      storage[j].first = storage[j-1].first;
      storage[j].second = storage[j-1].second + 1;
    }
    storage[position] = std::make_pair(values[i],offset);
  }
  return storage;

}
}  // namespace utils
}  // namespace buzzdb
