#pragma once
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace buzzdb {
namespace utils {

class Table : public std::vector<std::vector<std::string>> {
public:
  Table() : std::vector<std::vector<std::string>>() {}
  Table(std::initializer_list<std::vector<std::string>> list)
      : std::vector<std::vector<std::string>>(list) {}

  friend std::ostream &operator<<(std::ostream &os, const Table &table);
};

class InnerJoin {
public:
  InnerJoin(Table &left, Table &right, size_t left_attr_index,
            size_t right_attr_index);
  ~InnerJoin();

  friend std::ostream &operator<<(std::ostream &os, const Table &table);

  Table joined_table;

private:
  std::unordered_map<std::string, std::vector<std::string>> hashmap;
};

} // namespace utils
} // namespace buzzdb