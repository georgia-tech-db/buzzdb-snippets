
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

namespace buzzdb {
namespace utils {

#define UNUSED(p) ((void)(p))

void hello_world(){
  printf("Hello world");
}

vector<vector<std::string>> join(vector<vector<std::string>>& input_a, vector<vector<std::string>>& input_b, size_t col_a, size_t col_b) {
  vector<vector<std::string>> result;
  unordered_map<std::string, vector<vector<std::string>>> map_b;
  for (size_t i = 0; i < input_b.size(); i++) {
    auto key = input_b[i][col_b];
    if (map_b.find(key) == map_b.end()) {
      vector<vector<std::string>> added;
      map_b[input_b[i][col_b]] = added;
    }
    map_b[key].push_back(input_b[i]);
  }

  for (size_t i = 0; i < input_a.size(); i++) {
    auto key_a = input_a[i][col_a];
    if (map_b.find(key_a) != map_b.end()) {
      auto vector_b = map_b[key_a];
      for (size_t j = 0; j< vector_b.size(); j++) {
        vector<std::string> a_copy;
        for (size_t z = 0; z < input_a[i].size(); z++) {
          string a_str = input_a[i][z];
          a_copy.push_back(a_str);
        }

        // copy(input_a[i].begin(), input_a[i].begin(), a_copy.begin());
        for (size_t z = 0; z < vector_b[j].size(); z++) {
          a_copy.push_back(vector_b[j][z]);
        }
        result.push_back(a_copy);
      }
    }
  }
  return result;

}

}  // namespace utils
}  // namespace buzzdb
