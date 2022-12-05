
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
#include <iostream>
#include <fstream>

namespace buzzdb{

TEST(ExternalSortTests, BasicTest) {
    int64_t input[] = {3,2,5,1,4,6,9,8,0,7};
    int64_t outout[10];
    int64_t expected[] = {0,1,2,3,4,5,6,7,8,9};

    buzzdb::externalsort::external_sort((char *)&input, 10, (char *)&outout, 88);
    for (int i =0; i < 10; i++) {
        std::cout << outout[i] << "\n";
        EXPECT_EQ(outout[i], expected[i]);
    }
    

    buzzdb::externalsort::external_sort((char *)&input, 10, (char *)&outout, 88);
    for (int i =0; i < 10; i++) {
        EXPECT_EQ(outout[i], expected[i]);
    }

}

TEST(ExternalSortTests, OverflowTest) {
    int64_t input[] = {3,2,5,1,4,6};
    int64_t outout[10];
    int64_t expected[] = {1,2,3,4,5,6};

    buzzdb::externalsort::external_sort((char *)&input, 6, (char *)&outout, 16);

    for (int i =0; i < 6; i++) {
        std::cout << outout[i] << "\n";
        EXPECT_EQ(outout[i], expected[i]);
    }

    buzzdb::externalsort::external_sort((char *)&input, 6, (char *)&outout, 16);

    for (int i =0; i < 6; i++) {
        std::cout << outout[i] << "\n";
        EXPECT_EQ(outout[i], expected[i]);
    }

}

TEST(ExternalSortTests, LargeOverflowTest) {
    int64_t input[100];
    int64_t outout[100];
    int64_t expected[100];
    for (int i = 0; i < 100; i++) {
        input[i] = 100 - i;
        expected[i] = i + 1;
    }
    std::cout<< "input 99 " <<input[99]<<std::endl;

    buzzdb::externalsort::external_sort((char *)&input, 100, (char *)&outout, 60);

    for (int i =0; i < 100; i++) {
        std::cout << outout[i] << "\n";
        EXPECT_EQ(outout[i], expected[i]);
    }

    buzzdb::externalsort::external_sort((char *)&input, 100, (char *)&outout, 24);

    for (int i =0; i < 100; i++) {
        std::cout << outout[i] << "\n";
        EXPECT_EQ(outout[i], expected[i]);
    }
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

