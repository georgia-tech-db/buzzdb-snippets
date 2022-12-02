#include <gtest/gtest.h>
#include <cassert>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <vector>

#include "utils/run_length_encoding.h"

TEST(BasicTest, SimpleEncoding) {
    std::vector<std::pair<int, std::string>> r;
    std::pair<int, std::string> p;
    for (int i = 0; i < 50; i++) {
        if (i % 6 == 0) p = std::pair<int, std::string>(i, "M");
        else p = std::pair<int, std::string>(i, "F");
        r.push_back(p);
    }
    auto v = run_length_encoder(r);
    std::vector<int, encoding> vv;
    int j = 0;
    int same = 1;
    for (int i = 0; i < 17; i++) {
        encoding e;
        if (i % 2 == 0) {
            e.data = "M";
            if (i == 0) {
                e.i.first = 0;
                e.i.second = 1;
            } else {
                e.i.first = same;
                e.i.second = same;
                same += 4;
            }
        } else {
            e.data = "F";
            e.i.first = same;
            e.i.second = same + 4;
        }
        vv.push_back(i, e);
    }
    EXPECT_EQ(v, vv);
}