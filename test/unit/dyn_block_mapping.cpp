#include "dyn_block_mapping.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

namespace {

TEST(DynBlockMappingTests, BasicTest) {
  buzzdb::DynBlockMapping<8> dbm;
  EXPECT_EQ(dbm.num_segments(), 0);

  dbm.allocate(); // [0,7]
  EXPECT_EQ(dbm.num_segments(), 1);

  dbm.allocate(); // [8,17]
  EXPECT_EQ(dbm.num_segments(), 2);

  dbm[7].data.front() = 7;
  EXPECT_EQ(dbm[7].data.front(), 7);
  dbm[8].data.front() = 8;
  EXPECT_EQ(dbm[8].data.front(), 8);
  dbm[9].data.front() = 9;
  EXPECT_EQ(dbm[9].data.front(), 9);

  EXPECT_NO_THROW(dbm[17]);
  EXPECT_ANY_THROW(dbm[18]);
  EXPECT_ANY_THROW(dbm[19]);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
