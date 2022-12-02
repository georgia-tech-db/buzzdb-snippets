#include "../include/utils/innerjoin.h"
#include <iostream>
#include <string>
#include <vector>

#define UNUSED(p) ((void)(p))

namespace buzzdb {
namespace utils {
InnerJoin::InnerJoin(Table &left, Table &right, size_t left_attr_index,
                     size_t right_attr_index) {
  for (size_t i = 0; i < left.size(); i++) {
    hashmap[left[i][left_attr_index]] = left[i];
  }

  for (auto &tuple : right) {
    if (hashmap.find(tuple[right_attr_index]) != hashmap.end()) {
      std::vector<std::string> row = hashmap[tuple[right_attr_index]];
      row.insert(row.end(), tuple.begin(), tuple.end());

      joined_table.push_back(row);
    }
  }
}

InnerJoin::~InnerJoin() {}

std::ostream &operator<<(std::ostream &os, const buzzdb::utils::Table &table) {
  for (size_t i = 0; i < table.size(); i++) {
    for (size_t j = 0; j < table[i].size(); j++) {
      os << table[i][j] << '\t';
    }
    os << std::endl;
  }
  return os;
}

} // namespace utils
} // namespace buzzdb