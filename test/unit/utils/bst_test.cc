
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/bst.h"

namespace {

TEST(BstTest, InitializeTree) {
  buzzdb::BSTree::BSTree *tree = new buzzdb::BSTree::BSTree;
  delete tree;
}

TEST(BstTest, EmptyTree) {
  buzzdb::BSTree::BSTree *tree = new buzzdb::BSTree::BSTree;

  EXPECT_EQ(tree->IsEmpty(), true);
  EXPECT_EQ(tree->Insert(1), true);
  EXPECT_EQ(tree->IsEmpty(), false);
  EXPECT_EQ(tree->Delete(1), true);
  EXPECT_EQ(tree->IsEmpty(), true);

  delete tree;
}

TEST(BstTest, InsertDuplicate) {
  buzzdb::BSTree::BSTree *tree = new buzzdb::BSTree::BSTree;

  EXPECT_EQ(tree->Insert(1), true);
  EXPECT_EQ(tree->Insert(1), false);

  delete tree;
}

TEST(BstTest, DeleteNonexistant) {
  buzzdb::BSTree::BSTree *tree = new buzzdb::BSTree::BSTree;

  EXPECT_EQ(tree->Delete(2), false);
  EXPECT_EQ(tree->Insert(1), true);
  EXPECT_EQ(tree->Delete(2), false);
  EXPECT_EQ(tree->Delete(1), true);

  delete tree;
}

TEST(BstTest, Find) {
  buzzdb::BSTree::BSTree *tree = new buzzdb::BSTree::BSTree;

  EXPECT_EQ(tree->Find(2), false);
  EXPECT_EQ(tree->Insert(1), true);
  EXPECT_EQ(tree->Find(2), false);
  EXPECT_EQ(tree->Find(1), true);

  delete tree;
}

TEST(BstTest, Delete) {
  buzzdb::BSTree::BSTree *tree = new buzzdb::BSTree::BSTree;

  EXPECT_EQ(tree->Insert(5), true);
  EXPECT_EQ(tree->Insert(3), true);
  EXPECT_EQ(tree->Insert(4), true);
  EXPECT_EQ(tree->Insert(2), true);
  EXPECT_EQ(tree->Insert(7), true);
  EXPECT_EQ(tree->Insert(6), true);
  EXPECT_EQ(tree->Insert(8), true);

  EXPECT_EQ(tree->Delete(5), true);
  EXPECT_EQ(tree->Delete(4), true);
  EXPECT_EQ(tree->Delete(3), true);
  EXPECT_EQ(tree->Delete(2), true);
  EXPECT_EQ(tree->Delete(7), true);
  EXPECT_EQ(tree->Delete(8), true);

  delete tree;
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
