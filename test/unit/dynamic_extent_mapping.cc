#include "dynamic_extent_mapping.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

namespace {

TEST(DynamicExtentMappingTest, BasicTest) {
  // Initialize a new segment with an extent with 1 disk block of size 16 bytes
  // and a growth factor of 2
  buzzdb::Segment segment = buzzdb::Segment(1, 2);

  EXPECT_EQ(segment.get_num_extents(), 1);
  EXPECT_EQ(segment.extents[0]->capacity, 1);

  // this fills up extent #1 and creates extent #2 of size 2 disk blocks in the
  // segment
  segment.add_data("this is 16 bytes");
  EXPECT_EQ(segment.get_num_extents(), 2);
  EXPECT_EQ(segment.extents[1]->capacity, 2);

  // this fills up extent #2 and creates extent #3 of size 4 disk blocks
  segment.add_data("this is 16 bytes");
  segment.add_data("this is 16 bytes");
  EXPECT_EQ(segment.get_num_extents(), 3);
  EXPECT_EQ(segment.extents[2]->capacity, 4);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
