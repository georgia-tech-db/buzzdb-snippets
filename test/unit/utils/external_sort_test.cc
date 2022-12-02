
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/tutorial.h"
#include "utils/external_sort.h"

namespace {

TEST(ExternalSortTests, BasicTest) {
    int64_t input[] = {3,2,5,1,4,6};
    int64_t outout[6];
    int64_t expected[] = {1,2,3,4,5,6};

    buzzdb::externalsort::external_sort((char *)&input, 6, (char *)&outout, 12);

    EXPECT_EQ(outout, expected);

    buzzdb::externalsort::external_sort((char *)&input, 6, (char *)&outout, 6);

    EXPECT_EQ(outout, expected);

}

TEST(ExternalSortTests, OverflowTest) {
    int64_t input[] = {3,2,5,1,4,6};
    int64_t outout[6];
    int64_t expected[] = {1,2,3,4,5,6};

    buzzdb::externalsort::external_sort((char *)&input, 6, (char *)&outout, 3);

    EXPECT_EQ(outout, expected);

    buzzdb::externalsort::external_sort((char *)&input, 6, (char *)&outout, 2);

    EXPECT_EQ(outout, expected);

}

TEST(ExternalSortTests, LargeOverflowTest) {
    int64_t input[100];
    for (int i = 100; i > 0; i--) {
        input[i] = i;
    }
    int64_t outout[100];
    int64_t expected[100];
    for (int i = 1; i <= 100; i++) {
        expected[i] = i;
    }

    buzzdb::externalsort::external_sort((char *)&input, 100, (char *)&outout, 50);

    EXPECT_EQ(outout, expected);

    buzzdb::externalsort::external_sort((char *)&input, 100, (char *)&outout, 10);

    EXPECT_EQ(outout, expected);

}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

