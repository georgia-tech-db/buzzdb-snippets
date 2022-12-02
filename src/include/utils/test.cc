
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>

#include "utils/file.h"

namespace {

TEST(FileTest, BasicTest) {

   int randWays = 10;
   int mem = 1000;
   std::vector<uint64_t> values = {33, 22, 44, 55, 66, 2};
   char inputFile[] = "inputFile.txt";
   char outputFile[] = "outputFile.txt";
  std::unique_ptr<char[]> read_block(size_t offset, size_t size) {
    auto block = std::make_unique<char[]>(size);
    read_block(offset, size, block.get());
    return block;
  }
   FILE* input = openFile(inputFile, "w");

   srand(time(NULL));
   int x = 0;
   while (x < randWays * mem) {
      fprintf(input, "%d ", rand());
   }
   fclose(input);
   externalSort(inputFile, outputFile, randWays, mem);
   std::vector<uint64_t> values = {33, 22, 44, 55, 66, 2};
   std::sort(values.begin(), values.end());
  auto outValues = std::make_unique<uint64_t[]>(values.size());
  outputFile->read_block(0, fileSize, reinterpret_cast<char*>(outValues.get()));

  int y = 0;
  while (int y < values.size()) {
    EXPECT_EQ(values[y], outValues[y]);
    y++;
  }
  } 
}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
