#include <gtest/gtest.h>
#include "utils/SlottedPageScheme.h"

using SlottedPageScheme = buzzdb::SlottedPageScheme;



namespace {
    TEST(SlottedPageSchemeTest, InsertTuple) {
        //Header = 6 Bytes +  3 * 8  tuple max 
        SlottedPageScheme page(30);
        //expect 24 B of free space
        page.insertTuple(1);
        //expect 16 B of free space
        page.insertTuple(2);
        //expect 8B of free space
        page.insertTuple(3);
        //expect 0 B of free space
    }

    TEST(SlottedPageSchemeTest, DeleteTuple) {
        SlottedPageScheme page(30);
        
        page.insertTuple(1);
        page.insertTuple(2);
        page.insertTuple(3);
        //expect 0 B of free space
        page.deleteTuple(0);
        //expect 8B of free space
        page.deleteTuple(0);
        //expect 16B of free space
        page.deleteTuple(0);
        //expect 24B of free space

    }

    TEST(SlottedPageSchemeTest, FindTuple) {
        page.insertTuple(7);
        page.insertTuple(9);
        page.insertTuple(3);
        page.find(0);
        //find 7
        page.find(1);
        //find 9
        page.find(2);
        //find 3
        page.find(8);
        //dont find anything
    }

    


}
int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

