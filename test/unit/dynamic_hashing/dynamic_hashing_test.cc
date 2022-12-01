#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "dynamic_hashing/bucket.h"
#include "dynamic_hashing/directory.h"

namespace {

TEST(DynamicHashingTest, TestInitDirectory) {
  auto directory = buzzdb::utils::Directory(1); 
  EXPECT_EQ(directory.buckets[0]->mask, 1);
  EXPECT_EQ(directory.buckets[1]->mask, 1);
}

TEST(DynamicHashingTest, TestGetBucketID) {
  auto directory = buzzdb::utils::Directory(1); 
  EXPECT_EQ(directory.get_bucketid_by_val(0), 0);
  EXPECT_EQ(directory.get_bucketid_by_val(1), 1);
}


TEST(DynamicHashingTest, TestInsert) {
  auto directory = buzzdb::utils::Directory(1); 
  directory.insert(2);
  auto bucket0_vals = directory.buckets[0]->vals;
  std::vector<uint64_t> vec0{2};
  EXPECT_EQ(bucket0_vals, vec0);
  directory.insert(1);
  auto bucket1_vals = directory.buckets[1]->vals;
  EXPECT_EQ(bucket1_vals, std::vector<uint64_t>{1});
}

TEST(DynamicHashingTest, TestSplit) {
  auto directory = buzzdb::utils::Directory(1); 
  directory.insert(2);
  directory.insert(4);
  std::map<std::pair<uint64_t, size_t>, size_t> hash_relation = { 
    {std::make_pair(0, 2), 0},
    {std::make_pair(1, 2), 1},
    {std::make_pair(2, 2), 2},
    {std::make_pair(3, 2), 1},
  };
  EXPECT_EQ(directory.hash_relation, hash_relation);
  bool if_buckets_contain_3 = directory.buckets.find(3) != directory.buckets.end();
  EXPECT_EQ(if_buckets_contain_3, false);
  EXPECT_EQ(directory.buckets[0]->vals, std::vector<uint64_t>{4});
  EXPECT_EQ(directory.buckets[2]->vals, std::vector<uint64_t>{2});
}

TEST(DynamicHashingTest, TestGlobalDepthGreaterThanLocalDepthInsertSplit) {
  auto directory = buzzdb::utils::Directory(1); 
  directory.insert(2);
  directory.insert(4);

  directory.insert(3);
  EXPECT_EQ(directory.buckets[1]->vals, std::vector<uint64_t>{3});

  directory.insert(5);
  EXPECT_EQ(directory.buckets[3]->vals, std::vector<uint64_t>{3});
  EXPECT_EQ(directory.buckets[1]->vals, std::vector<uint64_t>{5});
}

TEST(DynamicHashingTest, TestMultipleInsertsAndSplits) {
  auto directory = buzzdb::utils::Directory(1); 
  directory.insert(10);
  directory.insert(7);
  directory.insert(0);
  directory.insert(6);
  directory.insert(5);
  directory.insert(4);
  directory.insert(8);
  EXPECT_EQ(directory.global_depth, 4);
  EXPECT_EQ(directory.hash_relation.size(), 16);
  EXPECT_EQ(directory.buckets.size(), 7);
  EXPECT_EQ(directory.buckets[8]->vals, std::vector<uint64_t>{8});

}

TEST(DynamicHashingTest, TestBucketSizeBiggerThanOne) {
  auto directory = buzzdb::utils::Directory(3); 
  directory.insert(10);
  directory.insert(7);
  directory.insert(0);
  directory.insert(6);
  directory.insert(5);
  directory.insert(4);
  directory.insert(8);
  EXPECT_EQ(directory.global_depth, 2);
  EXPECT_EQ(directory.hash_relation.size(), 4);
  EXPECT_EQ(directory.buckets.size(), 3);
  std::vector<uint64_t> vec0{0, 4 ,8};
  EXPECT_EQ(directory.buckets[0]->vals, vec0);
  std::vector<uint64_t> vec1{7, 5};
  EXPECT_EQ(directory.buckets[1]->vals, vec1);
}




}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
