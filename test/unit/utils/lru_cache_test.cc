#include <gtest/gtest.h>

#include "lru_cache.h"

namespace {

    TEST(TutorialTests, BasicTest) {
        auto tester = buzzdb::utils:LruCache();

        tester.addEntry("apple"); 2
        tester.addEntry("banana");
        tester.addEntry("orange"); 1
        tester.addEntry("apple");
        tester.addEntry("pear"); 3
 
        EXPECT_EQ(1, tester.getOrderOfEntry("orange"));
        EXPECT_EQ(2, tester.getOrderOfEntry("apple"));
        EXPECT_EQ(3, tester.getOrderOfEntry("pear"));
        EXPECT_EQ(-1, tester.getOrderOfEntry("banana"));
        EXPECT_EQ(-1, tester.getOrderOfEntry("strawberry"));

    }

}  

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}