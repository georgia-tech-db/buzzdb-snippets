#pragma once

#include <cstdint>
#include <memory>
#include <utility>

namespace buzzdb{
namespace utils{

std::vector<std::pair<int, int>> linearHashing(std::vector<std::pair<int, int>> hashTable, std::pair<int,int> pair);

}  // namespace utils
}  // namespace buzzdb
