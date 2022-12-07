#include <gtest/gtest.h>
#include <exception>
#include <utility>
#include <vector>
#include <iostream>

#include "dynamic_hashing/dynamic_hash_table.h"

namespace {
    TEST(HashTableTest, BasicSetupTest) {
        buzzdb::dynamic_hashing::HashTable map{3,1};
        EXPECT_EQ(map.size(), 0);
    }

    TEST(HashTableTest, BasicInsertTest) {
        buzzdb::dynamic_hashing::HashTable map{3,1};
        map.put(1, 5);
        map.put(2, 7);
        map.put(3, 17);
        EXPECT_EQ(map.size(), 3);
        EXPECT_EQ(map.get(1), 5);
        EXPECT_EQ(map.get(2), 7);
        EXPECT_EQ(map.get(3), 17);
    }

    TEST(HashTableTest, OverwriteTest) {
        buzzdb::dynamic_hashing::HashTable map{3,1};
        map.put(1, 5);
        map.put(2, 7);
        EXPECT_EQ(map.size(), 2);
        EXPECT_EQ(map.get(1), 5);
        EXPECT_EQ(map.get(2), 7);
        map.put(2, 18);
        EXPECT_EQ(map.get(2), 18);
    }

    TEST(HashTableTest, SplitTest) {
        buzzdb::dynamic_hashing::HashTable map{1,1};
        EXPECT_EQ(map.size(), 0);
        map.put(1, 3);
        map.put(2, 4);
        EXPECT_EQ(map.size(), 2);
        EXPECT_EQ(map.get(1), 3);
        EXPECT_EQ(map.get(2), 4);
    }

    TEST(HashTableTest, SplitMoreThanOnceTest) {
        buzzdb::dynamic_hashing::HashTable map{1,1};
        EXPECT_EQ(map.size(), 0);
        map.put(4, 3);
        map.put(8, 4);
        EXPECT_EQ(map.size(), 2);
        EXPECT_EQ(map.get(4), 3);
        EXPECT_EQ(map.get(8), 4);
        for (size_t i = 5; i < 8; i++) {
            map.put(i, i * 2);
            EXPECT_EQ(map.get(i), i * 2);
        }
        EXPECT_EQ(map.size(), 5);
        // make sure everything is still there
        EXPECT_EQ(map.get(4), 3);
        EXPECT_EQ(map.get(8), 4);
        for (size_t i = 5; i < 8; i++) {
            EXPECT_EQ(map.get(i), i * 2);
        }
    }

    TEST(HashTableTest, RemoveTest) {
        buzzdb::dynamic_hashing::HashTable map{1,1};
        EXPECT_EQ(map.size(), 0);
        map.put(1, 3);
        map.put(2, 4);
        map.remove(1);
        EXPECT_EQ(map.size(), 1);
        EXPECT_EQ(map.get(1).has_value(), false);
        map.remove(2);
        EXPECT_EQ(map.size(), 0);
        EXPECT_EQ(map.get(2).has_value(), false);
    }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}