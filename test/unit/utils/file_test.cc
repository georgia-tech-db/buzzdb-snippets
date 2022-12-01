#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <string>

#include "utils/file.h"

using namespace std;

namespace {

TEST(FileTest, HelloWorld) {
  buzzdb::utils::hello_world();
}

TEST(FileTest, BasicJoin) {
std::vector<std::vector<std::string>> input_a{ { "Anna", "12" }, 
                            { "Bella", "14" }, 
                            { "Tom", "8" } 
};

std::vector<std::vector<std::string>> input_b{ { "Anna", "black" }, 
                            { "Lily", "blue" }, 
                            { "Bella", "yellow"} 
};

std::vector<std::vector<std::string>> ans = buzzdb::utils::join(input_a, input_b, 0, 0);
for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = 0; j < ans[i].size(); j++)
        cout << ans[i][j] << " " << flush;
    cout << endl << flush;
}
std::vector<std::vector<std::string>> expected_output = { { "Anna", "12", "Anna", "black" }, 
                         { "Bella", "14", "Bella", "yellow" }
};
EXPECT_EQ(expected_output, ans);

}

TEST(FileTest, JoinWithDuplicateInTableB) {
std::vector<std::vector<std::string>> input_a{ { "Anna", "12" }, 
                            { "Bella", "14" }, 
                            { "Tom", "8" } 
};

std::vector<std::vector<std::string>> input_b{ { "Anna", "black" }, 
                            { "Anna", "blue" }, 
                            { "Bella", "yellow"} 
};

std::vector<std::vector<std::string>> ans = buzzdb::utils::join(input_a, input_b, 0, 0);
for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = 0; j < ans[i].size(); j++)
        cout << ans[i][j] << " " << flush;
    cout << endl << flush;
}
std::vector<std::vector<std::string>> expected_output = { { "Anna", "12", "Anna", "black" }, 
                         { "Anna", "12", "Anna", "blue" },
                         { "Bella", "14", "Bella", "yellow" }
};
EXPECT_EQ(expected_output, ans);

}

TEST(FileTest, JoinWithBothDuplicates) {
std::vector<std::vector<std::string>> input_a{ { "Anna", "12" }, 
                            { "Anna", "14" }
};

std::vector<std::vector<std::string>> input_b{ { "Anna", "black" }, 
                            { "Anna", "blue" }
};

std::vector<std::vector<std::string>> ans = buzzdb::utils::join(input_a, input_b, 0, 0);
for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = 0; j < ans[i].size(); j++)
        cout << ans[i][j] << " " << flush;
    cout << endl << flush;
}
std::vector<std::vector<std::string>> expected_output = { { "Anna", "12", "Anna", "black" }, 
                         { "Anna", "12", "Anna", "blue" },
                         { "Anna", "14", "Anna", "black" },
                         { "Anna", "14", "Anna", "blue" }
};
EXPECT_EQ(expected_output, ans);

}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
