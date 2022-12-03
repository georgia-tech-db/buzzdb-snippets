
#include <gtest/gtest.h>

#include "chained_hashing/chained_hashing.h"

namespace {

TEST(ChainedHashingTests, Test1) {

  auto table = buzzdb::utils::DynamicHashTable(3, false);
  table.insert("abc", "123");
  table.insert("abc", "456");

  EXPECT_EQ(table.get("abc"), "123");
  EXPECT_EQ(table.get("aaa"), buzzdb::utils::STRING_NOT_FOUND);
}

TEST(ChainedHashingTests, Test2) {

  auto table = buzzdb::utils::DynamicHashTable(3, false);
  table.insert("abc", "123");

  EXPECT_EQ(table.remove("abc"), true);
  EXPECT_EQ(table.get("abc"), buzzdb::utils::STRING_NOT_FOUND);
  EXPECT_EQ(table.remove("abc"), false);
}

TEST(ChainedHashingTests, Test3) {

  auto table = buzzdb::utils::DynamicHashTable(3, false);
  table.insert("abc", "123");
  table.insert("abc", "456");

  EXPECT_EQ(table.get("abc"), "123");
  EXPECT_EQ(table.remove("abc"), true);
  EXPECT_EQ(table.get("abc"), "456");
}

TEST(ChainedHashingTests, Test4) {

  auto table = buzzdb::utils::DynamicHashTable(3, true);
  table.insert("abc", "123");
  table.insert("abc", "456");

  EXPECT_EQ(table.get("abc"), "456");
  EXPECT_EQ(table.remove("abc"), true);
  EXPECT_EQ(table.get("abc"), buzzdb::utils::STRING_NOT_FOUND);
}

} // namespace


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}