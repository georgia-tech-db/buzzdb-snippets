#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "run_length_encode_string.h"

namespace {

TEST(RunLengthEncodeStringTest, EmptyString) {
  EXPECT_EQ(buzzdb::encode_string(""), "");
}

TEST(RunLengthEncodeStringTest, SingleCharOne) {
  EXPECT_EQ(buzzdb::encode_string("A"), "A1");
}

TEST(RunLengthEncodeStringTest, SingleCharTwo) {
  EXPECT_EQ(buzzdb::encode_string("AA"), "A2");
}

TEST(RunLengthEncodeStringTest, DoubleCharOne) {
  EXPECT_EQ(buzzdb::encode_string("AB"), "A1B1");
}

TEST(RunLengthEncodeStringTest, DoubleCharTwo) {
  EXPECT_EQ(buzzdb::encode_string("AABB"), "A2B2");
}

TEST(RunLengthEncodeStringTest, FlipFlop) {
  EXPECT_EQ(buzzdb::encode_string("ABAB"), "A1B1A1B1");
}

TEST(RunLengthEncodeStringTest, LongRun) {
  std::string long_run ("WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW");
  std::string long_run_exp ("W12B1W12B3W24B1W14");
  EXPECT_EQ(buzzdb::encode_string(long_run), long_run_exp);
}

TEST(RunLengthEncodeStringTest, MultipleCharLong) {
  EXPECT_EQ(buzzdb::encode_string("aaaabbcddddd"), "a4b2c1d5");
}


}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}