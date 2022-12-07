#include <gtest/gtest.h>
#include "utils/SlottedPageScheme.h"

using SlottedPageScheme = buzzdb::SlottedPageScheme;

namespace {
    TEST(SlottedPageSchemeTest, InsertTuple) {
        //Header = 6 Bytes +  3 * 8  tuple max 
        SlottedPageScheme page(30);
        //expect 24 B of free space
        EXPECT_EQ(page.header.free_space, 24);
        page.insertTuple(1);
        //expect 16 B of free space
        EXPECT_EQ(page.header.free_space, 16);
        page.insertTuple(2);
        //expect 8B of free space
        EXPECT_EQ(page.header.free_space, 8);
        page.insertTuple(3);
        //expect 0 B of free space
        EXPECT_EQ(page.header.free_space, 0);
    }

    TEST(SlottedPageSchemeTest, DeleteTuple) {
        SlottedPageScheme page(30);
        
        page.insertTuple(1);
        page.insertTuple(2);
        page.insertTuple(3);
        //expect 0 B of free space
        EXPECT_EQ(page.header.free_space, 0);
        page.deleteTuple(0);
        //expect 8B of free space
        EXPECT_EQ(page.header.free_space, 8);
        page.deleteTuple(0);
        //expect 16B of free space
        EXPECT_EQ(page.header.free_space, 16);
        page.deleteTuple(0);
        //expect 24B of free space
        EXPECT_EQ(page.header.free_space, 24);

    }

    TEST(SlottedPageSchemeTest, FindTuple) {
        page.insertTuple(7);
        page.insertTuple(9);
        page.insertTuple(3);
        EXPECT_EQ(page.find(0), 7);
        //find 7
        EXPECT_EQ(page.find(1), 9);
        //find 9
        EXPECT_EQ(page.find(2), 3);
        //find 3
        EXPECT_EQ(page.find(8), -1);
        //dont find anything
    }
}
int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

