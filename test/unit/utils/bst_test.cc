
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "./src/utils/bst.h"

namespace {

TEST(TestBst, TreeInitialization) {
  buzzdb::Tree::Tree *newBst = new buzzdb::Tree::Tree;
  delete newBst;
}

TEST(TestBst, TreeIsEmpty) {
  buzzdb::utils::Tree* bst = new buzzdb::utils::Tree();
  EXPECT_EQ(bst->IsEmpty(), true);
  delete tree;
}


TEST(testBst, DeleteNode) {
  buzzdb::Tree::Tree *bst = new buzzdb::Tree::Tree;
  bst->insert_node(bst->root, 1);
  bst->insert_node(bst->root, 2);
  bst->insert_node(bst->root, 3);
  bst->insert_node(bst->root, 4);
  bst->insert_node(bst->root, 5);

  EXPECT_EQ(bst->del_node(bst->root, 1), del_node(bst->root, 1)->data);
  EXPECT_EQ(bst->del_node(bst->root, 2), del_node(bst->root, 1)->data);
  EXPECT_EQ(bst->del_node(bst->root, 3), del_node(bst->root, 1)->data);
  EXPECT_EQ(bst->del_node(bst->root, 4), del_node(bst->root, 1)->data);
  EXPECT_EQ(bst->del_node(bst->root, 5), del_node(bst->root, 1)->data);

  delete tree;
}

TEST(testBst, InsertNode) {
  buzzdb::Tree::Tree *bst = new buzzdb::Tree::Tree;
  //check to see if the node's data matches
  EXPECT_EQ(bst->insert_node(bst->root, 1), insert_node(bst->root, 1)->data);
  EXPECT_EQ(bst->insert_node(bst->root, 2), insert_node(bst->root, 1)->data);
  EXPECT_EQ(bst->insert_node(bst->root, 3), insert_node(bst->root, 1)->data);
  EXPECT_EQ(bst->insert_node(bst->root, 4),insert_node(bst->root, 1)->data);
  EXPECT_EQ(bst->insert_node(bst->root, 5), insert_node(bst->root, 1)->data);
}

TEST(testBst, Search) {
  buzzdb::Tree::Tree *bst = new buzzdb::Tree::Tree;
  //check to see if the node's data matches!
  bst->insert_node(bst->root, 1);
  bst->insert_node(bst->root, 2);
  bst->insert_node(bst->root, 3);
  bst->insert_node(bst->root, 4);
  bst->insert_node(bst->root, 5);
  EXPECT_EQ(bst->search_node(bst->root, 5), search_node(bst->root, 5)->data);
}


}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
