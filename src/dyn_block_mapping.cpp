#include "dyn_block_mapping.h"

#include <stdexcept>

namespace buzzdb {

Segment::Segment(size_t capacity)
    : capacity{capacity},
      data_blocks{std::make_unique<DataBlock[]>(capacity)} {}

}  // namespace buzzdb
