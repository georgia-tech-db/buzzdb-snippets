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
        auto encoding = buzzdb::utils::DictEncoding("");
        EXPECT_EQ("", encoding.compress());
    }

    TEST(OperatorsTest, SameOrder) {
        auto encoding = buzzdb::utils::DictEncoding("Order: Vanilla, Vanilla, Vanilla");
        EXPECT_EQ("Order: 1, 1, 1", encoding.compress());
    }

    TEST(OperatorsTest, Different) {
        auto encoding = buzzdb::utils::DictEncoding("Order: Vanilla, Chocolate, Peppermint");
        EXPECT_EQ("Order: 1, 2, 5", encoding.compress());
    }
} // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}