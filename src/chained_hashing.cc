
#include "chained_hashing/chained_hashing.h"

namespace buzzdb{
namespace utils {

DynamicHashTable::DynamicHashTable(size_t num_buckets, bool uniqueKeys) : num_buckets(num_buckets),  
                                                          uniqueKeys(uniqueKeys),
                                                          table(num_buckets) {}

std::string DynamicHashTable::get(std::string key) {
  int keyHash = std::hash<std::string>{}(key) % num_buckets;
  for (auto& p : table[keyHash]) {
    if (p.first == key) {
      return p.second;
    }
  }
  return STRING_NOT_FOUND;
}

void DynamicHashTable::insert(std::string key, std::string val) {
  int keyHash = std::hash<std::string>{}(key) % num_buckets;
  if (uniqueKeys) {
    for (auto& p : table[keyHash]) {
      if (p.first == key) {
        p.second = val;
        return;
      }
    }
  }
  table[keyHash].push_back(std::make_pair(key, val));
}

bool DynamicHashTable::remove(std::string key) {
  int keyHash = std::hash<std::string>{}(key) % num_buckets;
  auto it = table[keyHash].begin();
  while (it != table[keyHash].end()) {
    if (it->first == key) {
      table[keyHash].erase(it);
      return true;
    }
    ++it;
  }
  return false;
}

}
}