
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

TEST(FileTest, HelloWorld) {
  buzzdb::utils::hello_world();
}

TEST(FileTest, BasicJoinTest) {
  std::vector<std::vector<int>> input1 = {{1, 10, 100, 1000}, {2, 20, 200, 2000}};
  std::vector<std::vector<int>> input2 = {{9, 99, 1, 999}, {8, 88, 2, 888}};
  size_t jk_idx_input1 = 0;
  size_t jk_idx_input2 = 2;

  std::vector<std::vector<int>> output = buzzdb::utils::join(input1, input2, jk_idx_input1, jk_idx_input2);
  std::vector<std::vector<int>> expected_output =
    {{1, 10, 100, 1000, 9, 99, 999}, {2, 20, 200, 2000, 8, 88, 888}};

  EXPECT_EQ(output.size(), expected_output.size());

  for (size_t i = 0; i < output.size(); i++) {
    EXPECT_EQ(output[i], expected_output[i]);
  }
}

TEST(FileTest, DuplicateJoinTest) {
  std::vector<std::vector<int>> input1 = {{1, 10, 100, 1000}, {2, 20, 200, 2000}};
  std::vector<std::vector<int>> input2 = {{9, 99, 1, 999}, {8, 88, 2, 888}, {8, 88, 2, 888}};
  size_t jk_idx_input1 = 0;
  size_t jk_idx_input2 = 2;

  std::vector<std::vector<int>> output = buzzdb::utils::join(input1, input2, jk_idx_input1, jk_idx_input2);
  std::vector<std::vector<int>> expected_output =
    {{1, 10, 100, 1000, 9, 99, 999}, {2, 20, 200, 2000, 8, 88, 888}};

  EXPECT_EQ(output.size(), expected_output.size());

  for (size_t i = 0; i < output.size(); i++) {
    EXPECT_EQ(output[i], expected_output[i]);
  }
}

TEST(FileTest, EmptyResultJoinTest) {
  std::vector<std::vector<int>> input1 = {{1, 10, 100, 1000}, {2, 20, 200, 2000}};
  std::vector<std::vector<int>> input2 = {};
  size_t jk_idx_input1 = 0;
  size_t jk_idx_input2 = 2;

  std::vector<std::vector<int>> output = buzzdb::utils::join(input1, input2, jk_idx_input1, jk_idx_input2);
  std::vector<std::vector<int>> expected_output =
    {{1, 10, 100, 1000}, {2, 20, 200, 2000}};

  EXPECT_EQ(output.size(), expected_output.size());

  for (size_t i = 0; i < output.size(); i++) {
    EXPECT_EQ(output[i], expected_output[i]);
  }
}

TEST(FileTest, EmptyJoinTest) {
  std::vector<std::vector<int>> input1 = {};
  std::vector<std::vector<int>> input2 = {};
  size_t jk_idx_input1 = 0;
  size_t jk_idx_input2 = 2;

  std::vector<std::vector<int>> output = buzzdb::utils::join(input1, input2, jk_idx_input1, jk_idx_input2);
  std::vector<std::vector<int>> expected_output = {};

  EXPECT_EQ(output.size(), expected_output.size());

  for (size_t i = 0; i < output.size(); i++) {
    EXPECT_EQ(output[i], expected_output[i]);
  }
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
