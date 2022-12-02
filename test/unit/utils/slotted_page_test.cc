#include <gtest/gtest.h>

#include "utils/slotted_page.h"

using SlottedPage = buzzdb::SlottedPage;

namespace {

TEST(SlottedPageTest, AddSlot) {
    SlottedPage new_page(32);
    new_page.addSlot(45);
    new_page.addSlot(69);
    new_page.addSlot(67);

    EXPECT_EQ(45, new_page.slots[0].stored_value);
    EXPECT_EQ(69, new_page.slots[1].stored_value);
    EXPECT_EQ(67, new_page.slots[2].stored_value);
}

TEST(SlottedPageTest, RemoveWholeSlotBasic) {
    int removeIndex = 1;
    SlottedPage new_page(32);
    new_page.addSlot(45);
    new_page.addSlot(69);
    new_page.addSlot(67);
    new_page.removeSlot(removeIndex);

    EXPECT_EQ(45, new_page.slots[0].stored_value);
    EXPECT_EQ(67, new_page.slots[1].stored_value);
}

TEST(SlottedPageTest, RemoveValueBasic) {
    uint32_t removeValue = 67;
    SlottedPage new_page(32);
    new_page.addSlot(45);
    new_page.addSlot(69);
    new_page.addSlot(67);
    new_page.removeSlot(removeValue);

    EXPECT_EQ(45, new_page.slots[0].stored_value);
    EXPECT_EQ(69, new_page.slots[1].stored_value);
    EXPECT_EQ(-1, new_page.slots[2].stored_value);
    EXPECT_EQ(2, new_page.slot_header.next_free_slot);
}

TEST(SlottedPageTest, RemoveValueThenAddNoFreeSlots) {
    uint32_t removeValue = 67;
    SlottedPage new_page(32);
    new_page.addSlot(45);
    new_page.addSlot(69);
    new_page.addSlot(67);
    new_page.removeSlot(removeValue);

    EXPECT_EQ(45, new_page.slots[0].stored_value);
    EXPECT_EQ(69, new_page.slots[1].stored_value);
    EXPECT_EQ(-1, new_page.slots[2].stored_value);
    EXPECT_EQ(2, new_page.slot_header.next_free_slot);

    new_page.addSlot(42);

    EXPECT_EQ(45, new_page.slots[0].stored_value);
    EXPECT_EQ(69, new_page.slots[1].stored_value);
    EXPECT_EQ(42, new_page.slots[2].stored_value);
    EXPECT_EQ((uint16_t) -1, new_page.slot_header.next_free_slot);
}

TEST(SlottedPageTest, RemoveValueThenAddWithFreeSlot) {
    uint32_t removeValue1 = 67;
    uint32_t removeValue2 = 69;
    SlottedPage new_page(32);
    new_page.addSlot(45);
    new_page.addSlot(69);
    new_page.addSlot(67);
    new_page.removeSlot(removeValue1);
    new_page.removeSlot(removeValue2);

    EXPECT_EQ(45, new_page.slots[0].stored_value);
    EXPECT_EQ(-1, new_page.slots[1].stored_value);
    EXPECT_EQ(-1, new_page.slots[2].stored_value);
    EXPECT_EQ(1, new_page.slot_header.next_free_slot);

    new_page.addSlot(42);

    EXPECT_EQ(45, new_page.slots[0].stored_value);
    EXPECT_EQ(42, new_page.slots[1].stored_value);
    EXPECT_EQ(-1, new_page.slots[2].stored_value);
    EXPECT_EQ(2, new_page.slot_header.next_free_slot);
}

}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}