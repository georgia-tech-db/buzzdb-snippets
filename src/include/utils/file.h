#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

namespace buzzdb{
namespace utils{

void hello_world();

std::vector<std::vector<std::string>> join(vector<vector<string>>& input_a, vector<vector<string>>& input_b, size_t col_a, size_t col_b);

}  // namespace utils
}  // namespace buzzdb
