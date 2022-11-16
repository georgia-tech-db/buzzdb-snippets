
#include <gtest/gtest.h>
// #include <algorithm>
// #include <cstdint>
// #include <cstring>
// #include <exception>
// #include <random>
// #include <utility>
// #include <vector>

#include "readWriteLatch.h"

namespace {

    TEST(FileTest, HelloWorld) {
        buzzdb::readWriteLatchExample();
    }

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
