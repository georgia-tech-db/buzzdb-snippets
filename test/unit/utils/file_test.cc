
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

TEST(FileTest, robin_hood_hashing) {
  int size = 7;
  std::vector<int> values;
  values.push_back(2);
  values.push_back(0);
  values.push_back(9);
  values.push_back(10);
  values.push_back(16);
  values.push_back(5);
  auto output = buzzdb::utils::robin_hood_hashing(values,size);
  std::vector<std::pair<int, int>> expected;
  expected.push_back(std::make_pair(0,0));
  expected.push_back(std::make_pair(-1,-1));
  expected.push_back(std::make_pair(2,0));
  expected.push_back(std::make_pair(9,1));
  expected.push_back(std::make_pair(16,2));
  expected.push_back(std::make_pair(10,2));
  expected.push_back(std::make_pair(5,1));
  EXPECT_EQ(expected, output);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
