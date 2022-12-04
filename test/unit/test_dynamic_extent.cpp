#include "include/segment.h"

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
  buzzdb::Segment segment = buzzdb::Segment();

  EXPECT_EQ(segment.get_extent_num();, 0);

  for(int ii = 0; ii < 16 * 16 * 3; ii++){
    segment.write(ii);
  }
  EXPECT_EQ(segment.get_extent_num(), 3);
  EXPECT_EQ(segment.extents[1].size, 16);

  // this fills up extent #2 and creates extent #3 of size 4 disk blocks
  segment.write(16 * 16 * 3 + 1);
  EXPECT_EQ(segment.get_extent_num(), 4);
  EXPECT_EQ(segment.extents[3]->capacity, 1);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}