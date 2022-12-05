#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/linearhash.h"

namespace {

TEST(LinearHashTests, InsertTest) {
  auto table = buzzdb::utils::LinearHashTable(10);

  int test_values[] = {13, 0, 1, 2, 3, 4, 23, 31};

  for (int x : test_values) {
    EXPECT_EQ(table.insert_item(x, x), true);
    table.print();
  }

  for (int x : test_values) {
    int val = table.find_item(x);
    EXPECT_EQ(val, x);
  }
}


TEST(LinearHashTests, DeleteTest) {
  auto table = buzzdb::utils::LinearHashTable(10);

  int x = 0;
  for (size_t i = 0; i < table.capacity; i++) {
    EXPECT_EQ(table.insert_item(x, x), true);
    x += table.capacity;
  }

  table.print();

  int k = 0;
  for (size_t i = 0; i < table.capacity; i++) {
    EXPECT_EQ(table.delete_item(k), true);
    k += table.capacity;
  table.print();
  }
}


} // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}