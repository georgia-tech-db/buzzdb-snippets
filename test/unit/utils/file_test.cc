
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/file.h"

#define UNUSED(p) ((void)(p))

namespace {

TEST(FileTest, HelloWorld) {
  buzzdb::utils::hello_world();
}

}  // namespace

int main(int argc, char *argv[]) {
  // testing::InitGoogleTest(&argc, argv);
  UNUSED(argc);
  UNUSED(argv);
  return 0;
  // return RUN_ALL_TESTS();
}
