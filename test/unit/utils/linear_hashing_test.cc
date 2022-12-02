
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/linear_hashing.h"

// using buzzdb::utils::LinearHashTable;

namespace {

TEST(LinearHashTableTests, InitTable) {
  buzzdb::utils::LinearHashTable::Entry ee = buzzdb::utils::LinearHashTable::Entry(0, 0, 0, 0);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {ee, ee, ee, ee, ee};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);

  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, InsertOne) {
  buzzdb::utils::LinearHashTable::Entry ee = buzzdb::utils::LinearHashTable::Entry(0, 0, 0, 0);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {ee, onefive, ee, ee, ee};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(1, 5);

  EXPECT_EQ(1, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, InsertFive) {
  buzzdb::utils::LinearHashTable::Entry ee = buzzdb::utils::LinearHashTable::Entry(0, 0, 0, 0);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothree = buzzdb::utils::LinearHashTable::Entry(2, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry threesix = buzzdb::utils::LinearHashTable::Entry(3, 6, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fournine = buzzdb::utils::LinearHashTable::Entry(4, 9, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fivetwo = buzzdb::utils::LinearHashTable::Entry(5, 2, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {ee, onefive, twothree, threesix, fournine, fivetwo, ee, ee, ee, ee};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(10);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(3, 6);
  table.insert(4, 9);
  table.insert(5, 2);

  EXPECT_EQ(5, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, InsertFull) {
  buzzdb::utils::LinearHashTable::Entry zerotwo = buzzdb::utils::LinearHashTable::Entry(0, 2, 0, 1);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothree = buzzdb::utils::LinearHashTable::Entry(2, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry threesix = buzzdb::utils::LinearHashTable::Entry(3, 6, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fournine = buzzdb::utils::LinearHashTable::Entry(4, 9, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {zerotwo, onefive, twothree, threesix, fournine};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(0, 2);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(3, 6);
  table.insert(4, 9);

  EXPECT_EQ(5, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, InsertOverflow) {
  buzzdb::utils::LinearHashTable::Entry zerotwo = buzzdb::utils::LinearHashTable::Entry(0, 2, 0, 1);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothree = buzzdb::utils::LinearHashTable::Entry(2, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry threesix = buzzdb::utils::LinearHashTable::Entry(3, 6, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fournine = buzzdb::utils::LinearHashTable::Entry(4, 9, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {zerotwo, onefive, twothree, threesix, fournine};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(0, 2);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(3, 6);
  table.insert(4, 9);
  table.insert(5, 2);

  EXPECT_EQ(5, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, InsertReplace) {
  buzzdb::utils::LinearHashTable::Entry ee = buzzdb::utils::LinearHashTable::Entry(0, 0, 0, 0);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {ee, onefive, ee, ee, ee};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(1, 8);
  table.insert(1, 5);

  EXPECT_EQ(1, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, InsertFiveCollision) {
  buzzdb::utils::LinearHashTable::Entry ee = buzzdb::utils::LinearHashTable::Entry(0, 0, 0, 0);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry eleventhree = buzzdb::utils::LinearHashTable::Entry(11, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twentyone_six = buzzdb::utils::LinearHashTable::Entry(21, 6, 0, 1);
  buzzdb::utils::LinearHashTable::Entry thirtyone_nine = buzzdb::utils::LinearHashTable::Entry(31, 9, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fortyone_two = buzzdb::utils::LinearHashTable::Entry(41, 2, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {ee, onefive, eleventhree, twentyone_six, thirtyone_nine, fortyone_two, ee, ee, ee, ee};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(10);
  table.insert(1, 5);
  table.insert(11, 3);
  table.insert(21, 6);
  table.insert(31, 9);
  table.insert(41, 2);

  EXPECT_EQ(5, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, InsertThreeOneCollision) {
  buzzdb::utils::LinearHashTable::Entry ee = buzzdb::utils::LinearHashTable::Entry(0, 0, 0, 0);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothree = buzzdb::utils::LinearHashTable::Entry(2, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry elevensix = buzzdb::utils::LinearHashTable::Entry(11, 6, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {ee, onefive, twothree, elevensix, ee, ee, ee, ee, ee, ee};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(10);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(11, 6);

  EXPECT_EQ(3, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, EraseOne) {
  buzzdb::utils::LinearHashTable::Entry zerotwo = buzzdb::utils::LinearHashTable::Entry(0, 2, 0, 1);
  buzzdb::utils::LinearHashTable::Entry onefivets = buzzdb::utils::LinearHashTable::Entry(1, 5, 1, 1);
  buzzdb::utils::LinearHashTable::Entry twothree = buzzdb::utils::LinearHashTable::Entry(2, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry threesix = buzzdb::utils::LinearHashTable::Entry(3, 6, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fournine = buzzdb::utils::LinearHashTable::Entry(4, 9, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {zerotwo, onefivets, twothree, threesix, fournine};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(0, 2);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(3, 6);
  table.insert(4, 9);
  table.erase(1);

  EXPECT_EQ(4, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, EraseAll) {
  buzzdb::utils::LinearHashTable::Entry zerotwots = buzzdb::utils::LinearHashTable::Entry(0, 2, 1, 1);
  buzzdb::utils::LinearHashTable::Entry onefivets = buzzdb::utils::LinearHashTable::Entry(1, 5, 1, 1);
  buzzdb::utils::LinearHashTable::Entry twothreets = buzzdb::utils::LinearHashTable::Entry(2, 3, 1, 1);
  buzzdb::utils::LinearHashTable::Entry threesixts = buzzdb::utils::LinearHashTable::Entry(3, 6, 1, 1);
  buzzdb::utils::LinearHashTable::Entry fourninets = buzzdb::utils::LinearHashTable::Entry(4, 9, 1, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {zerotwots, onefivets, twothreets, threesixts, fourninets};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(0, 2);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(3, 6);
  table.insert(4, 9);
  table.erase(0);
  table.erase(1);
  table.erase(2);
  table.erase(3);
  table.erase(4);

  EXPECT_EQ(0, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, EraseThreeOneCollision) {
  buzzdb::utils::LinearHashTable::Entry ee = buzzdb::utils::LinearHashTable::Entry(0, 0, 0, 0);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothreets = buzzdb::utils::LinearHashTable::Entry(2, 3, 1, 1);
  buzzdb::utils::LinearHashTable::Entry elevensix = buzzdb::utils::LinearHashTable::Entry(11, 6, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {ee, onefive, twothreets, elevensix, ee, ee, ee, ee, ee, ee};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(10);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(11, 6);
  table.erase(2);

  EXPECT_EQ(2, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, EraseKeyNotFound) {
  buzzdb::utils::LinearHashTable::Entry zerotwo = buzzdb::utils::LinearHashTable::Entry(0, 2, 0, 1);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothree = buzzdb::utils::LinearHashTable::Entry(2, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry threesix = buzzdb::utils::LinearHashTable::Entry(3, 6, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fournine = buzzdb::utils::LinearHashTable::Entry(4, 9, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {zerotwo, onefive, twothree, threesix, fournine};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(0, 2);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(3, 6);
  table.insert(4, 9);
  
  EXPECT_EQ(-1, table.erase(5));
  EXPECT_EQ(5, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
}

TEST(LinearHashTableTests, LookupOne) {
  buzzdb::utils::LinearHashTable::Entry zerotwo = buzzdb::utils::LinearHashTable::Entry(0, 2, 0, 1);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothree = buzzdb::utils::LinearHashTable::Entry(2, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry threesix = buzzdb::utils::LinearHashTable::Entry(3, 6, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fournine = buzzdb::utils::LinearHashTable::Entry(4, 9, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {zerotwo, onefive, twothree, threesix, fournine};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(0, 2);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(3, 6);
  table.insert(4, 9);

  EXPECT_EQ(5, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
  EXPECT_EQ(5, table.lookup(1));
}

TEST(LinearHashTableTests, LookupEraseThreeOneCollision) {
  buzzdb::utils::LinearHashTable::Entry ee = buzzdb::utils::LinearHashTable::Entry(0, 0, 0, 0);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothreets = buzzdb::utils::LinearHashTable::Entry(2, 3, 1, 1);
  buzzdb::utils::LinearHashTable::Entry elevensix = buzzdb::utils::LinearHashTable::Entry(11, 6, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {ee, onefive, twothreets, elevensix, ee, ee, ee, ee, ee, ee};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(10);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(11, 6);
  table.erase(2);

  EXPECT_EQ(2, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
  EXPECT_EQ(6, table.lookup(11));
}

TEST(LinearHashTableTests, LookupKeyNotFound) {
  buzzdb::utils::LinearHashTable::Entry zerotwo = buzzdb::utils::LinearHashTable::Entry(0, 2, 0, 1);
  buzzdb::utils::LinearHashTable::Entry onefive = buzzdb::utils::LinearHashTable::Entry(1, 5, 0, 1);
  buzzdb::utils::LinearHashTable::Entry twothree = buzzdb::utils::LinearHashTable::Entry(2, 3, 0, 1);
  buzzdb::utils::LinearHashTable::Entry threesix = buzzdb::utils::LinearHashTable::Entry(3, 6, 0, 1);
  buzzdb::utils::LinearHashTable::Entry fournine = buzzdb::utils::LinearHashTable::Entry(4, 9, 0, 1);
  std::vector<buzzdb::utils::LinearHashTable::Entry> v = {zerotwo, onefive, twothree, threesix, fournine};

  buzzdb::utils::LinearHashTable table = buzzdb::utils::LinearHashTable(5);
  table.insert(0, 2);
  table.insert(1, 5);
  table.insert(2, 3);
  table.insert(3, 6);
  table.insert(4, 9);

  EXPECT_EQ(5, table.size());
  EXPECT_EQ(v, table.get_backing_vector());
  EXPECT_EQ(-1, table.lookup(5));
}


}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
