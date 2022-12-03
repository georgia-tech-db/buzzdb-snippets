#pragma once

#include <vector>
#include <list>
#include <utility>
#include <string>
#include <functional>

namespace buzzdb{
namespace utils{

const std::string STRING_NOT_FOUND = "STRING_NOT_FOUND";

class DynamicHashTable {
private:
  const size_t num_buckets;
  const bool uniqueKeys;
  std::vector<std::list<std::pair<std::string, std::string>>> table;
  
public:
  DynamicHashTable(size_t num_buckets, bool uniqueKeys);

  std::string get(std::string key);

  void insert(std::string key, std::string val);

  bool remove(std::string key);
};

}
}