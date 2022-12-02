#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/linearHash.h"

namespace {

TEST(LinearHashTest, TestTableAppending) {
    vector<pair<int, int>> myVec (100, std::make_pair(-1, -1));
    vector<pair<int, int>> testVec (100, std::make_pair(-1, -1));
    myVec[1] = std::make_pair(1, 10);
    myVec[2] = std::make_pair(102, 9);

    testVec = buzzdb::utils::linearHashing(testVec, std::make_pair(1,10));
    testVec = buzzdb::utils::linearHashing(testVec, std::make_pair(102,9));

    EXPECT_EQ(myVec, testVec);
}

TEST(LinearHashTest, TestLinearProbe) {
    vector<pair<int, int>> myVec (10, std::make_pair(-1, -1));
    vector<pair<int, int>> testVec (10, std::make_pair(-1, -1));

    myVec[1] = std::make_pair(1, 10);
    myVec[2] = std::make_pair(11, 9);

    testVec = buzzdb::utils::linearHashing(testVec, std::make_pair(1,10));
    testVec = buzzdb::utils::linearHashing(testVec, std::make_pair(11,9));

    EXPECT_EQ(myVec, testVec);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}