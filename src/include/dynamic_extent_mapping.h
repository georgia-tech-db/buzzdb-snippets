#include <array>
#include <memory>
#include <vector>

namespace buzzdb {

class DiskBlock {
 public:
  std::array<char, 16> data;  // each disk block stores 16 bytes of data
  uint8_t size;
  uint8_t capacity;

  DiskBlock();
};

class Extent {
 public:
  std::vector<DiskBlock *> disk_blocks;
  uint32_t size;
  uint32_t capacity;

  Extent(uint64_t num_disk_blocks);

  ~Extent();
};

class Segment {
 public:
  std::vector<Extent *> extents;
  uint64_t initial_extent_num_disk_blocks;
  double growth_factor;
  uint64_t size;
  int power;

  Segment(uint64_t initial_extent_num_disk_blocks, double growth_factor);

  ~Segment();

  void add_data(std::string s);

  size_t get_num_extents();
};

}  // namespace buzzdb