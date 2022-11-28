#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "bplus_tree_latch.h"

namespace {

TEST(BplusTreeTest, ReadLatchTest) {
  buzzdb::BPlusTreeLatchDemo tester;
  tester.runReadOp(20);
}

TEST(BplusTreeTest, InsertLatchTest) {
  buzzdb::BPlusTreeLatchDemo tester;
  tester.runInsertOp(6);
  tester.runInsertOp(21);
}

TEST(BplusTreeTest, DeleteLatchTest) {
  buzzdb::BPlusTreeLatchDemo tester;
  tester.runDeleteOp(20);
  tester.runDeleteOp(10);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}