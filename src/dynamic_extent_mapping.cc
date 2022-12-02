#include "dynamic_extent_mapping.h"

#include <math.h>

#include <array>
#include <vector>

namespace buzzdb {

DiskBlock::DiskBlock() : size(0), capacity(16){};

Extent::Extent(uint64_t num_disk_blocks) : size(0), capacity(num_disk_blocks) {
  for (uint64_t i = 0; i < num_disk_blocks; i++) {
    DiskBlock* block = new DiskBlock();
    disk_blocks.push_back(block);
  }
};

Extent::~Extent() {
  for (DiskBlock* db : disk_blocks) delete db;
  disk_blocks.clear();
};

Segment::Segment(uint64_t initial_extent_num_disk_blocks, double growth_factor)
    : initial_extent_num_disk_blocks(initial_extent_num_disk_blocks),
      growth_factor(growth_factor),
      size(0),
      power(1) {
  Extent* extent = new Extent(initial_extent_num_disk_blocks);
  extents.push_back(extent);
};

Segment::~Segment() {
  for (Extent* e : extents) delete e;
  extents.clear();
}

void Segment::add_data(std::string s) {
  std::vector<char> chars(s.begin(), s.end());

  Extent* extent = extents[size];
  DiskBlock* disk_block = extent->disk_blocks[extent->size];
  for (char c : chars) {
    disk_block->data[disk_block->size] = c;

    if (++(disk_block->size) == disk_block->capacity) {
      if (++(extent->size) == extent->capacity) {
        size++;
        Extent* new_extent =
            new Extent((uint64_t)(initial_extent_num_disk_blocks *
                                  pow(growth_factor, power++)));
        extents.push_back(new_extent);

        extent = extents[size];
      }
      disk_block = extent->disk_blocks[extent->size];
    }
  }
};

size_t Segment::get_num_extents() { return size + 1; };

}  // namespace buzzdb