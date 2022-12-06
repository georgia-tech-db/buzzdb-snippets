#include <gtest/gtest.h>

#include "utils/join.h"

namespace {

TEST(JoinTests, BasicJoin) {
    buzzdb::join::Table left({{"1", "2", "3"}, {"4", "5", "6"}});
    buzzdb::join::Table right({{"1", "7", "8"}, {"4", "9", "10"}});
    buzzdb::join::Join join{left, right, 0, 0};
    buzzdb::join::Table expected({{"1", "2", "3", "7", "8"}, {"4", "5", "6", "9", "10"}});
    EXPECT_EQ(join.result_table.data, expected.data);
}

TEST(JoinTests, NoJoin) {
    buzzdb::join::Table left({{"1", "2", "3"}, {"4", "5", "6"}});
    buzzdb::join::Table right({{"7", "8", "9"}, {"10", "11", "12"}});
    buzzdb::join::Join join{left, right, 0, 0};
    size_t expected_joined_table_length = 0;
    EXPECT_EQ(join.result_table.data.size(), expected_joined_table_length);
}
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
