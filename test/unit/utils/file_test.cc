
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/file.h"

namespace {

TEST(FileTest, InsertTest) {
  auto RH_table = buzzdb::utils::RHT(5);

  std::cout << "Table created" << std::endl;
  for (size_t i = 0; i < RH_table.table_size; i++) {
    EXPECT_EQ(RH_table.insert(i, i), true);
  }
  EXPECT_NE(RH_table.insert(6, 6), true);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
