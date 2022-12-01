#include <gtest/gtest.h>
#include <algorithm>
#include <atomic>
#include <cstring>
#include <memory>
#include <random>
#include <thread>
#include <vector>

#include "utils/buffermanager.h"

namespace {

TEST(BufferManagerTest, FixSingle) {
  buzzdb::BufferManager buffer_manager{1024, 10};
  std::vector<uint64_t> expected_values(1024 / sizeof(uint64_t), 123);
  {
    auto& page = buffer_manager.fix_page(1, true);
    std::memcpy(page.get_data(), expected_values.data(), 1024);
    buffer_manager.unfix_page(page, true);
    EXPECT_EQ(std::vector<uint64_t>{1}, buffer_manager.get_fifo_list());

  }
  {
    std::vector<uint64_t> values(1024 / sizeof(uint64_t));
    auto& page = buffer_manager.fix_page(1, false);
    std::memcpy(values.data(), page.get_data(), 1024);
    buffer_manager.unfix_page(page, true);
    EXPECT_EQ(std::vector<uint64_t>{1}, buffer_manager.get_fifo_list());
    ASSERT_EQ(expected_values, values);
  }
}

TEST(BufferManagerTest, FixEleven) {
  buzzdb::BufferManager buffer_manager{1024, 10};
  std::vector<uint64_t> expected_values(1024 / sizeof(uint64_t), 123);
  {
    auto& page1 = buffer_manager.fix_page(1, true);
    buffer_manager.unfix_page(page1, true);
    auto& page2 = buffer_manager.fix_page(2, true);
    buffer_manager.unfix_page(page2, true);
    auto& page3 = buffer_manager.fix_page(3, true);
    buffer_manager.unfix_page(page3, true);
    auto& page4 = buffer_manager.fix_page(4, true);
    buffer_manager.unfix_page(page4, true);
    auto& page5 = buffer_manager.fix_page(5, true);
    buffer_manager.unfix_page(page5, true);
    auto& page6 = buffer_manager.fix_page(6, true);
    buffer_manager.unfix_page(page6, true);
    auto& page7 = buffer_manager.fix_page(7, true);
        buffer_manager.unfix_page(page7, true);

    auto& page8 = buffer_manager.fix_page(8, true);
        buffer_manager.unfix_page(page8, true);
    auto& page9 = buffer_manager.fix_page(9, true);
        buffer_manager.unfix_page(page9, true);
    auto& page10 = buffer_manager.fix_page(10, true);
        buffer_manager.unfix_page(page10, true);
    auto& page11 = buffer_manager.fix_page(11, true);
        buffer_manager.unfix_page(page11, true);

    EXPECT_EQ(11, buffer_manager.get_fifo_list()[9]);
  }
}

TEST(BufferManagerTest, PersistentRestart) {
  auto buffer_manager = std::make_unique<buzzdb::BufferManager>(1024, 10);
  for (uint16_t segment = 0; segment < 3; ++segment) {
    for (uint64_t segment_page = 0; segment_page < 10; ++segment_page) {
      uint64_t page_id = (static_cast<uint64_t>(segment) << 48) | segment_page;
      auto& page = buffer_manager->fix_page(page_id, true);
      uint64_t& value = *reinterpret_cast<uint64_t*>(page.get_data());
      value = segment * 10 + segment_page;
      buffer_manager->unfix_page(page, true);
    }
  }
  buffer_manager = std::make_unique<buzzdb::BufferManager>(1024, 10);
  for (uint16_t segment = 0; segment < 3; ++segment) {
    for (uint64_t segment_page = 0; segment_page < 10; ++segment_page) {
      uint64_t page_id = (static_cast<uint64_t>(segment) << 48) | segment_page;
      auto& page = buffer_manager->fix_page(page_id, false);
      uint64_t value = *reinterpret_cast<uint64_t*>(page.get_data());
      buffer_manager->unfix_page(page, false);
      EXPECT_EQ(segment * 10 + segment_page, value);
    }
  }
}

TEST(BufferManagerTest, FIFOEvict) {
  buzzdb::BufferManager buffer_manager{1024, 10};
  for (uint64_t i = 1; i < 11; ++i) {
    auto& page = buffer_manager.fix_page(i, false);
    buffer_manager.unfix_page(page, false);
  }
  {
    std::vector<uint64_t> expected_fifo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    EXPECT_EQ(expected_fifo, buffer_manager.get_fifo_list());

  }
  {
    auto& page = buffer_manager.fix_page(11, false);
    buffer_manager.unfix_page(page, false);
  }
  {
    std::vector<uint64_t> expected_fifo{2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    EXPECT_EQ(expected_fifo, buffer_manager.get_fifo_list());

  }
}

TEST(BufferManagerTest, BufferFull) {
  buzzdb::BufferManager buffer_manager{1024, 10};
  std::vector<buzzdb::BufferFrame*> pages;
  pages.reserve(10);
  for (uint64_t i = 1; i < 11; ++i) {
    auto& page = buffer_manager.fix_page(i, false);
    pages.push_back(&page);
  }
  EXPECT_THROW(buffer_manager.fix_page(11, false), buzzdb::buffer_full_error);
}


}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}