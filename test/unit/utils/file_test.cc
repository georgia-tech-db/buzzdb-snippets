
#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "utils/file.h"

namespace {

TEST(FileTest, EncodingTest) {
  std::string str = "wwwwaaadexxxxxxywww";
  std::vector<char> input;
  int n = str.length();
  for (int i = 0; i < n; i++) {
      input.push_back(static_cast<char>(str[i]));
  }

  auto result = buzzdb::utils::run_length_encoding(input);
  /*
  for (auto e: result) {
    std::cout << "char: " << e.c << "offset: " << e.offset << "count: " << e.count << '\n';
  }
  */
  std::vector<buzzdb::utils::Element> expected;
  buzzdb::utils::Element e1 = {'w', 0, 4};
  buzzdb::utils::Element e2 = {'a', 4, 3};
  buzzdb::utils::Element e3 = {'d', 7, 1};
  buzzdb::utils::Element e4 = {'e', 8, 1};
  buzzdb::utils::Element e5 = {'x', 9, 6};
  buzzdb::utils::Element e6 = {'y', 15, 1};
  buzzdb::utils::Element e7 = {'w', 16, 3};
  expected.push_back(e1);
  expected.push_back(e2);
  expected.push_back(e3);
  expected.push_back(e4);
  expected.push_back(e5);
  expected.push_back(e6);
  expected.push_back(e7);
  EXPECT_EQ(expected, result);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
