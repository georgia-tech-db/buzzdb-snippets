#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace buzzdb{
namespace utils{

void hello_world();

std::vector<std::vector<int>> join(std::vector<std::vector<int>>& input1,
    std::vector<std::vector<int>>& input2, size_t jk_idx_input1, size_t jk_idx_input2);
}  // namespace utils
}  // namespace buzzdb
