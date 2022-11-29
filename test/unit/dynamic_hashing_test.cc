#include <gtest/gtest.h>

#include "dynamic_hashing.h"

namespace {

TEST(DynamicHashingTests, BasicInsertTest) {

  // Create a new hash table with bucket size = 2
  buzzdb::directory dir(2);

  // Add four key-value pairs to fill the initial buckets completely
  dir.insert(4, "val1");
  dir.insert(5, "val2");
  dir.insert(11, "val3");
  dir.insert(14, "val4");

  EXPECT_EQ(dir.get_global_depth(), 1);
}

TEST(DynamicHashingTests, BucketSplitInsertTest1) {

  // Create a new hash table with bucket size = 2
  buzzdb::directory dir(2);

  // Add four key-value pairs to fill the initial buckets completely
  dir.insert(4, "val1");
  dir.insert(5, "val2");
  dir.insert(11, "val3");
  dir.insert(14, "val4");

  EXPECT_EQ(dir.get_global_depth(), 1);

  // Adding this key will cause the bucket to
  // split, and the depth will increase
  dir.insert(7, "val5");

  EXPECT_EQ(dir.get_global_depth(), 2);
}

TEST(DynamicHashingTests, BucketSplitInsertTest2) {

  // Create a new hash table with bucket size = 2
  buzzdb::directory dir(2);

  // Add four key-value pairs to fill the initial buckets completely
  dir.insert(4, "val1");
  dir.insert(5, "val2");
  dir.insert(11, "val3");
  dir.insert(14, "val4");

  EXPECT_EQ(dir.get_global_depth(), 1);

  // Adding this key will cause the bucket to
  // split, and the depth will increase
  dir.insert(7, "val5");

  EXPECT_EQ(dir.get_global_depth(), 2);

  // Adding this key will also split buckets,
  // but depth will not change
  dir.insert(12, "val6");

  EXPECT_EQ(dir.get_global_depth(), 2);
}

TEST(DynamicHashingTests, LookupTest) {

  // Create a new hash table with bucket size = 2
  buzzdb::directory dir(2);

  // Add some key-value pairs, and ensure buckets split
  dir.insert(4, "val1");
  dir.insert(5, "val2");
  dir.insert(11, "val3");
  dir.insert(14, "val4");
  dir.insert(7, "val5");
  dir.insert(12, "val6");

  EXPECT_EQ(dir.lookup(7), "val5");
  EXPECT_FALSE(dir.lookup(41).has_value());
}

} // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
