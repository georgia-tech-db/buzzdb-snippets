#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/run_length_encoding.h"

namespace {

TEST(OperatorsTest, EmptyString) {
    auto encoding = buzzdb::utils::RunLengthEncoding("");
    EXPECT_EQ("", encoding.compress());
}

TEST(OperatorsTest, AllEqual) {
    auto encoding = buzzdb::utils::RunLengthEncoding("aaaaaaa");
    EXPECT_EQ("7a", encoding.compress());
}

TEST(OperatorsTest, Different) {
    auto encoding = buzzdb::utils::RunLengthEncoding("aabaaa");
    EXPECT_EQ("2a1b3a", encoding.compress());
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}