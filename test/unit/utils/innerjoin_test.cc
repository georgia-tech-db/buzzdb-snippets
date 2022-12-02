#include "utils/innerjoin.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

namespace {
using buzzdb::utils::InnerJoin;
using buzzdb::utils::Table;

Table relation_students{
    // ID, Name, Age, Major
    {"1", "Alice", "19", "CS"},   {"2", "Jonah", "20", "EE"},
    {"3", "Charlie", "19", "CS"}, {"4", "David", "22", "EE"},
    {"5", "Eve", "18", "CS"},
};

Table relation_grades{
    // ID, Course, Grade
    {"1", "CS101", "A"},
    {"1", "EE200", "B"},
    {"2", "CS101", "C"},
    {"4", "EE200", "B"},
};

TEST(AlgorithmTest, InnerJoin) {
  const Table expected_table = Table{
      // ID, Name, Age, Major, Course, Grade
      {"1", "Alice", "19", "CS", "1", "CS101", "A"},
      {"1", "Alice", "19", "CS", "1", "EE200", "B"},
      {"2", "Jonah", "20", "EE", "2", "CS101", "C"},
      {"4", "David", "22", "EE", "4", "EE200", "B"},
  };

  InnerJoin join{relation_students, relation_grades, 0, 0};

  EXPECT_EQ(join.joined_table, expected_table);

  std::cout << "Expected table:" << std::endl << expected_table << std::endl;
  std::cout << "Joined table:" << std::endl << join.joined_table << std::endl;
}

} // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
