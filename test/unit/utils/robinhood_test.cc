#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/robinhood.h"

namespace {

TEST(RobinhoodTests, BasicInsertTest) {
  auto robin_table = buzzdb::utils::RobinHoodHashTable(10);

  std::cout << robin_table << std::endl;

  for (size_t i = 0; i < robin_table.capacity; i++) {
    EXPECT_EQ(robin_table.insert_item(i, i), true);
  }

  EXPECT_EQ(robin_table.insert_item(10, 10), false);
  std::cout << robin_table << std::endl;
}

TEST(RobinhoodTests, InsertCollisionTest) {
  auto robin_table = buzzdb::utils::RobinHoodHashTable(10);

  std::cout << robin_table << std::endl;

  int x = 0;
  for (size_t i = 0; i < robin_table.capacity; i++) {
    EXPECT_EQ(robin_table.insert_item(x,x), true);
    buzzdb::utils::RobinHoodHashTable::Entry* temp = robin_table.find_item(x);
    EXPECT_EQ(temp->key, x);
    EXPECT_EQ(temp->value, x);

    x += robin_table.capacity;
  }

  std::cout << robin_table << std::endl;

}

TEST(RobinhoodTests, BasicRobinHoodInsertTest) {
  auto robin_table = buzzdb::utils::RobinHoodHashTable(10);

  int test_values[] = {0, 10, 1, 20};
  std::cout << robin_table << std::endl;

  for (int x : test_values) {
    EXPECT_EQ(robin_table.insert_item(x,x), true);
    std::cout << robin_table << std::endl;
  }

  for (int x : test_values) {
    buzzdb::utils::RobinHoodHashTable::Entry* temp = robin_table.find_item(x);
    EXPECT_EQ(temp->key, x);
    EXPECT_EQ(temp->value, x);
  }
}

TEST(RobinhoodTests, BasicDeleteTest) {
  auto robin_table = buzzdb::utils::RobinHoodHashTable(10);

  std::cout << robin_table << std::endl;

  EXPECT_EQ(robin_table.insert_item(0, 0), true);
  EXPECT_EQ(robin_table.insert_item(1, 1), true);

  std::cout << robin_table << std::endl;

  EXPECT_EQ(robin_table.delete_item(1), true);

  std::cout << robin_table << std::endl;
}

TEST(RobinhoodTests, BackShiftDeleteTest) {
  auto robin_table = buzzdb::utils::RobinHoodHashTable(10);

  std::cout << robin_table << std::endl;

  int x = 0;
  for (size_t i = 0; i < robin_table.capacity; i++) {
    EXPECT_EQ(robin_table.insert_item(x,x), true);
    x += robin_table.capacity;
  }

  std::cout << robin_table << std::endl;

  int k = 0;
  for (size_t i = 0; i < robin_table.capacity; i++) {
    EXPECT_EQ(robin_table.delete_item(k), true);
    k += robin_table.capacity;
    std::cout << robin_table << std::endl;
  }
}

TEST(RobinhoodTests, FindDeletedItemTest) {
  auto robin_table = buzzdb::utils::RobinHoodHashTable(10);

  EXPECT_EQ(robin_table.insert_item(0,0), true);
  EXPECT_EQ(robin_table.insert_item(10,10), true);
  EXPECT_EQ(robin_table.insert_item(20,10), true);

  EXPECT_EQ(robin_table.delete_item(10), true);

  buzzdb::utils::RobinHoodHashTable::Entry* temp = robin_table.find_item(10);

  EXPECT_TRUE(temp == NULL);
}

} // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}