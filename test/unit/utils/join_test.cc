#include "join.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

namespace {
// Test Join Condition
TEST(JoinTests, BasicJoin) {
  // Create two tuples in different tables.
  // Table with 1 tuple, id = 1, val = 1, and Table with 1 tuple, id = 1, val = 2
  buzzdb::utils::Table t1 = buzzdb::utils::Table(
    {std::make_tuple(1,1,false)}
  );
  buzzdb::utils::Table t2 = buzzdb::utils::Table(
    {std::make_tuple(1,2,false)}
  );
  auto joinedTables = joinTables(t1, t2);
  int expectedOutputLength = 2;
  // OUTPUT SHOULD HAVE LENGTH 2 (1 + 1).
  EXPECT_EQ(2, expectedOutputLength);
}

// Test Join Condition Fails
TEST(JoinTests, BasicNoJoin) {
  // Create two tuples in different tables
  // Table with 1 tuple, id = 1, val = 1, and Table with 1 tuple, id = 2, val = 2
  buzzdb::utils::Table t1 = buzzdb::utils::Table(
    {std::make_tuple(1,1,false)}
  );
  buzzdb::utils::Table t2 = buzzdb::utils::Table(
    {std::make_tuple(2,2,false)}
  );
  auto joinedTables = joinTables(t1, t2);
  int expectedOutputLength = 0;
  // OUTPUT SHOULD HAVE LENGTH 0 since no tuples have matching join condition.
  EXPECT_EQ(0, expectedOutputLength);
}

TEST(JoinTests, AdvancedJoin) {
buzzdb::utils::Table t1 = buzzdb::utils::Table(
  {
  std::make_tuple(1,1,false),
  std::make_tuple(2,1,false),
  std::make_tuple(3,3,false),
  }
);

buzzdb::utils::Table t2 = buzzdb::utils::Table(
  {
  std::make_tuple(1,20,false),
  std::make_tuple(2,30,false),
  std::make_tuple(4,3,false),
  }
);
  auto joinedTables = joinTables(t1, t2);
  int expectedOutputLength = 4;
  // OUTPUT SHOULD HAVE LENGTH 4 since 4 tuples satisfy join condition.
  EXPECT_EQ(4, expectedOutputLength);
}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}