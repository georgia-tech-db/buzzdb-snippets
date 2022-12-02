
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

TEST(FileTest, EncodingTest) {
  std::string str = "aaabbcc";

  std::string result = buzzdb::utils::run_length_encoding(input);
  std::string expected = "a3b2c2";
  
  EXPECT_EQ(expected, result);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
