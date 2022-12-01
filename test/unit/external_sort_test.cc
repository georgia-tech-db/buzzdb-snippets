#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "external_sort.h"
#include "file_wrapper.h"

namespace {

TEST(SortTests, BasicSortTest) {
  std::vector<uint64_t> vals = {43, 22, 34, 53, 64, 12};

  auto file_size = vals.size() * sizeof(uint64_t);

  auto temp_test_file = File::make_temporary_file();
  temp_test_file->resize(file_size);
  temp_test_file->write_block(reinterpret_cast<char*>(vals.data()), 0, file_size);

  auto temp_out_file = File::make_temporary_file();

  // will have only one chunk
  external_sort(*temp_test_file, vals.size(), *temp_out_file, 1000);

  auto readout_vals = std::make_unique<uint64_t[]>(vals.size());
  temp_out_file->read_block(0, file_size, reinterpret_cast<char*>(readout_vals.get()));
  
  std::sort(vals.begin(), vals.end());

  for (size_t i = 0; i < vals.size(); i++) {
    EXPECT_EQ(vals[i], readout_vals[i]);
  }
}

TEST(SortTests, OddChunksSortTest) {
  std::vector<uint64_t> vals;

  for (int i = 100; i >= 0; i--) {
    vals.push_back(i);
  }

  auto file_size = vals.size() * sizeof(uint64_t);

  auto temp_test_file = File::make_temporary_file();
  temp_test_file->resize(file_size);
  temp_test_file->write_block(reinterpret_cast<char*>(vals.data()), 0, file_size);

  auto temp_out_file = File::make_temporary_file();

  // will have 5 chunks
  external_sort(*temp_test_file, vals.size(), *temp_out_file, 168);

  auto readout_vals = std::make_unique<uint64_t[]>(vals.size());
  temp_out_file->read_block(0, file_size, reinterpret_cast<char*>(readout_vals.get()));

  for (uint64_t i = 0; i <= 100; i++) {
    EXPECT_EQ(i, readout_vals[i]);
  }
}

TEST(SortTests, EvenChunksSortTest) {
  std::vector<uint64_t> vals;

  for (int i = 100; i >= 0; i--) {
    vals.push_back(i);
  }

  auto file_size = vals.size() * sizeof(uint64_t);

  auto temp_test_file = File::make_temporary_file();
  temp_test_file->resize(file_size);
  temp_test_file->write_block(reinterpret_cast<char*>(vals.data()), 0, file_size);

  auto temp_out_file = File::make_temporary_file();

  // will have 4 chunks
  external_sort(*temp_test_file, vals.size(), *temp_out_file, 208);

  auto readout_vals = std::make_unique<uint64_t[]>(vals.size());
  temp_out_file->read_block(0, file_size, reinterpret_cast<char*>(readout_vals.get()));

  for (uint64_t i = 0; i <= 100; i++) {
    EXPECT_EQ(i, readout_vals[i]);
  }
}

} // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}