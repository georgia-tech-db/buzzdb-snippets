#include "segment_extent_data.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

namespace {

TEST(SEDTests, BasicTest) {
  buzzdb::utils::Extent e(5);
  EXPECT_EQ(e.is_overflowing(), false);
  
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}