#include <gtest/gtest.h>
#include <numeric>
#include <thread>
#include <time.h>
#include <unistd.h>

#include "utils/bplustree.h"

namespace {
TEST(BplusTree, SingleThreadInsertIncreasing) {
    for (int degree = 2; degree < 10; degree++) {
        int numberOfInserts = 25;
        buzzdb::BTree bTree(degree);
        for (int i = 1; i <= numberOfInserts; ++i) {
            bTree.insert(i);
        }

        std::vector<int> sortedKeys;
        std::vector<int> expectedSortedKeys(numberOfInserts);
        std::iota(expectedSortedKeys.begin(), expectedSortedKeys.end(), 1);
        bTree.display(sortedKeys);

        EXPECT_EQ(expectedSortedKeys, sortedKeys);
    }
}

TEST(BplusTree, SingleThreadInsertDecreasing) {
    for (int degree = 2; degree < 10; degree++) {
        int numberOfInserts = 25;
        buzzdb::BTree bTree(degree);
        for (size_t i = numberOfInserts; i > 0; --i) {
            bTree.insert(i);
        }

        std::vector<int> sortedKeys;
        std::vector<int> expectedSortedKeys(numberOfInserts);
        std::iota(expectedSortedKeys.begin(), expectedSortedKeys.end(), 1);
        bTree.display(sortedKeys);

        EXPECT_EQ(expectedSortedKeys, sortedKeys);
    }
}

TEST(BplusTree, MultiThreadInsertIncreasing) {
    for (int degree = 2; degree < 10; degree++) {
        int numberOfInserts = 25;
        buzzdb::BTree bTree(degree);

        std::vector<std::thread> threads;
        for (int i = 1; i <= numberOfInserts; ++i) {
            threads.emplace_back([i, &bTree] {
                bTree.insert(i);
                usleep(1);
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        std::vector<int> sortedKeys;
        std::vector<int> expectedSortedKeys(numberOfInserts);
        std::iota(expectedSortedKeys.begin(), expectedSortedKeys.end(), 1);
        bTree.display(sortedKeys);

        EXPECT_EQ(expectedSortedKeys, sortedKeys);
    }
}

TEST(BplusTree, MultiThreadInsertDecreasing) {
    for (int degree = 2; degree < 10; degree++) {
        int numberOfInserts = 25;
        buzzdb::BTree bTree(degree);

        std::vector<std::thread> threads;

        for (size_t i = numberOfInserts; i > 0; --i) {
            threads.emplace_back([i, &bTree] {
                bTree.insert(i);
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        std::vector<int> sortedKeys;
        std::vector<int> expectedSortedKeys(numberOfInserts);
        std::iota(expectedSortedKeys.begin(), expectedSortedKeys.end(), 1);
        bTree.display(sortedKeys);

        EXPECT_EQ(expectedSortedKeys, sortedKeys);
    }
}

TEST(BplusTree, MultiThreadInsertRandom) {
    for (int degree = 2; degree < 10; degree++) {
        int numberOfInserts = 25;
        buzzdb::BTree bTree(degree);

        std::vector<std::thread> threads;

        std::vector<int> expectedSortedKeys(numberOfInserts);
        srand(time(0));
        int random;

        for (size_t i = numberOfInserts; i > 0; --i) {
            random = rand();
            expectedSortedKeys.push_back(random);
        }

        for (auto num : expectedSortedKeys) {
            threads.emplace_back([num, &bTree] {
                bTree.insert(num);
            });
        }

        std::sort(expectedSortedKeys.begin(), expectedSortedKeys.end());

        for (auto& thread : threads) {
            thread.join();
        }

        std::vector<int> sortedKeys;
        bTree.display(sortedKeys);

        EXPECT_EQ(expectedSortedKeys, sortedKeys);
    }
}

} // namespace

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
