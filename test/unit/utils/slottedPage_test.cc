#include <gtest/gtest.h>

#include "utils/slottedPage.h"

using SlottedPage = buzzdb::SlottedPage;

namespace {

TEST(SlottedPageTest, InsertRecord) {
    std::cout << "hererrrrr\n";
    //size of header = 8 + 3 * 4 = 20
    SlottedPage new_page(20);
    EXPECT_EQ(8, new_page.header.first_free_slot);
    new_page.insertRecord(1);
    EXPECT_EQ(4, new_page.header.first_free_slot);
    new_page.insertRecord(2);
    EXPECT_EQ(0, new_page.header.first_free_slot);
    new_page.insertRecord(3);
    
    for (auto slot : new_page.slots) {
        std::cout << slot.value << "value inserted\n";
    }
    EXPECT_EQ(1, new_page.slots[0].value);
    EXPECT_EQ(2, new_page.slots[1].value);
    EXPECT_EQ(3, new_page.slots[2].value);
}

TEST(SlottedPageTest, deleteLastRecord) {
  SlottedPage new_page(24);
  new_page.insertRecord(1);
  new_page.insertRecord(2);
  new_page.insertRecord(3);
  new_page.deleteRecord(2);
  for (auto slot : new_page.slots) {
        std::cout << slot.value << "value inserted\n";
  }
  
  EXPECT_EQ(1, new_page.slots[0].value);
  EXPECT_EQ(2, new_page.slots[1].value);
  EXPECT_EQ(2, new_page.slots.size());

}

TEST(SlottedPageTest, RemoveMidRecord) {
  SlottedPage new_page(24);
  new_page.insertRecord(1);
  new_page.insertRecord(2);
  new_page.insertRecord(3);
  new_page.deleteRecord(1);
  
  
  EXPECT_EQ(1, new_page.slots[0].value);
  EXPECT_EQ(1111, new_page.slots[1].value);
  EXPECT_EQ(3, new_page.slots[2].value);
}

TEST(SlottedPageTest, RemoveMidRecordAndAddNewValue) {
  SlottedPage new_page(24);
  new_page.insertRecord(1);
  new_page.insertRecord(2);
  new_page.insertRecord(3);
  new_page.deleteRecord(1);
  new_page.insertRecord(4);
  for (auto slot : new_page.slots) {
        std::cout << slot.value << "value inserted\n";
  }
  EXPECT_EQ(1, new_page.slots[0].value);
  EXPECT_EQ(4, new_page.slots[1].value);
  EXPECT_EQ(3, new_page.slots[2].value);
  EXPECT_EQ(3, new_page.header.slot_count);
}
TEST(SlottedPageTest, lookUpRecord) {
  SlottedPage new_page(24);
  new_page.insertRecord(1);
  new_page.insertRecord(2);
  new_page.insertRecord(3);
  new_page.insertRecord(4);
  EXPECT_EQ(3, new_page.lookUpRecord(2));
}
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}