
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
  for (int i = 0; i < RH_table.table_size; i++) {
    EXPECT_EQ(RH_table.insert(i, i), true);
  }
  std::cout << "out of loop" << std::endl;
  EXPECT_NE(RH_table.insert(6, 6), true);
}
TEST(FileTest, SearchKeyFoundTest) {
  auto RH_table = buzzdb::utils::RHT(5);

  std::cout << "Table created" << std::endl;
    RH_table.insert(2, 2);
  // Key found
  pair<int, bool> search_result = RH_table.search(2);
  EXPECT_EQ(search_result.first, 2);
  EXPECT_EQ(search_result.second, true);                                                                           
}
TEST(FileTest, SearchKeyNotFoundTest) {
  auto RH_table = buzzdb::utils::RHT(5);

  std::cout << "Table created" << std::endl;
  RH_table.insert(1, 1);
  // Key not found
  pair<int, bool> search_result = RH_table.search(2);
  EXPECT_EQ(search_result.first, -1);
  EXPECT_EQ(search_result.second, false);                                                                           
}
TEST(FileTest, DeleteTest) {
  auto RH_table = buzzdb::utils::RHT(5);

  std::cout << "Table created" << std::endl;
  RH_table.insert(1, 1);
  // erase the key
  RH_table.erase(1);
  // Key not found
  pair<int, bool> search_result = RH_table.search(1);
  EXPECT_EQ(search_result.first, -1);
  EXPECT_EQ(search_result.second, false);                                                                           
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
