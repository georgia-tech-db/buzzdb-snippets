#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

namespace buzzdb {

struct DataBlock {
  std::array<uint8_t, sizeof(nullptr)> data;
};

struct Segment {
  explicit Segment(size_t capacity);

  const size_t capacity;
  std::unique_ptr<DataBlock[]> data_blocks;
};

template <size_t INITIAL_SEG_SIZE = 128, std::intmax_t GROW_FACTOR_NOM = 125,
          std::intmax_t GROW_FACTOR_DENOM = 100>
struct DynBlockMapping {
  std::vector<Segment> extent;

  Segment& allocate() {
    if (extent.empty()) {
      extent.emplace_back(INITIAL_SEG_SIZE);
    } else {
      auto& last_seg = extent.back();
      extent.emplace_back(static_cast<size_t>(
          1. * last_seg.capacity * GROW_FACTOR_NOM / GROW_FACTOR_DENOM));
    }
    return extent.back();
  }

  DataBlock& operator[](size_t index) const {
    size_t seg_idx = 0;
    while (seg_idx < extent.size()) {
      if (index < extent[seg_idx].capacity)
        return extent[seg_idx].data_blocks[index];
      index -= extent[seg_idx].capacity;
      ++seg_idx;
    }

    throw std::runtime_error("Index out of bounds");
  }

  auto num_segments() const { return extent.size(); }
};

};  // namespace buzzdb
