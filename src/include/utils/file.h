#pragma once

#include <cstdint>
#include <memory>

namespace buzzdb{
namespace utils{


int hash_function(size_t k, size_t size);
std::vector<std::pair<int, int>> robin_hood_hashing(std::vector<int> values, int size);

}  // namespace utils
}  // namespace buzzdb
