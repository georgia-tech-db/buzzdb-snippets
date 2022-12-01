
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>
#include "BST.h"
#include "utils/file.h"

namespace {

TEST(BST, EmptyTree) {
  buzzdb::utils::BST* tree = new buzzdb::utils::BST();
  EXPECT_EQ(NULL, tree->getRoot());
  delete tree;
}

TEST(BST, insertOneElement) {
  buzzdb::utils::BST* tree = new buzzdb::utils::BST();
  tree->insert(50);
  EXPECT_EQ(tree->getRoot()->data, 50);
  delete tree;
}

TEST(BST, getCorrectRoot) {
  buzzdb::utils::BST* tree = new buzzdb::utils::BST();
  tree->insert(2);
  tree->insert(5);
  EXPECT_EQ(2, tree->getRoot()->data);
  EXPECT_EQ(5, tree->getRoot()->right->data);
  delete tree;
}

TEST(BST, preOrderTraversalTest) {
  buzzdb::utils::BST* tree = new buzzdb::utils::BST();
  tree->insert(15);
  tree->insert(22);
  tree->insert(4);
  tree->insert(7);
  tree->insert(1);
  tree->insert(30);
  tree->insert(16);
  std::string str = "";
  EXPECT_EQ("15 4 1 7 22 16 30 ", tree->preOrder(tree->getRoot(), str));
  delete tree;
}

TEST(BST, postOrderTraversalTest) {
  buzzdb::utils::BST* tree = new buzzdb::utils::BST();
  tree->insert(15);
  tree->insert(22);
  tree->insert(4);
  tree->insert(7);
  tree->insert(1);
  tree->insert(30);
  tree->insert(16);
  std::string str = "";
  EXPECT_EQ("1 7 4 16 30 22 15 ", tree->postOrder(tree->getRoot(), str));
  delete tree;
}

TEST(BST, inOrderTraversalTest) {
  buzzdb::utils::BST* tree = new buzzdb::utils::BST();
  tree->insert(15);
  tree->insert(22);
  tree->insert(4);
  tree->insert(7);
  tree->insert(1);
  tree->insert(30);
  tree->insert(16);
  std::string str = "";
  EXPECT_EQ("1 4 7 15 16 22 30 ", tree->inOrder(tree->getRoot(), str));
  delete tree;
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}