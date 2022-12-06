#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "buffer_manager.h"

namespace {

typedef std::pair<size_t, uint64_t> frame;

TEST(BufferManagerTest, BasicTest) {
    std::string page_id_string = "1 2 3 1 1 1 2 2 2 4 5 6 7 8 1 1 2 3 2 2 1 4 5 4 5 4 5";
    std::vector<uint64_t> page_ids = buzzdb::parse_input(page_id_string);

    std::vector<frame> fifo_output = buzzdb::fifo(page_ids);
    std::vector<frame> lru_output = buzzdb::lru(page_ids);
    std::pair<std::vector<frame>, std::vector<frame>> two_queue_output = buzzdb::two_queue(page_ids);

    std::vector<frame> expected_fifo_output = {std::make_pair(2, 3), std::make_pair(3, 4), std::make_pair(1, 5)};
    std::vector<frame> expected_lru_output = {std::make_pair(2, 1), std::make_pair(1, 4), std::make_pair(3, 5)};
    std::vector<frame> expected_fifo_queue = {std::make_pair(3, 5)};
    std::vector<frame> expected_lru_queue = {std::make_pair(2, 2), std::make_pair(1, 1)};

    EXPECT_EQ(fifo_output, expected_fifo_output);
    EXPECT_EQ(lru_output, expected_lru_output);
    EXPECT_EQ(two_queue_output.first, expected_fifo_queue);
    EXPECT_EQ(two_queue_output.second, expected_lru_queue);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}