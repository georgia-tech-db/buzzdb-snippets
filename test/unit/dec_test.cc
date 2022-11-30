

#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>
#include <string>

#include "src/include/dec.h"

using Entry = buzzdb::dec::Entry<std::string>;
using CompressedEntry = buzzdb::dec::CompressedEntry;
using Dictionary = buzzdb::dec::Dictionary<std::string>;
namespace {

TEST(DECTests, EmptyDictionaryTest) {
    std::vector<Entry> entries;
    Dictionary dictionary(entries);
    EXPECT_EQ(dictionary.size(),0);
}

TEST(DECTests, SingleEntryCompressionTest) {
    std::vector<Entry> entries;
    Entry t{"aaa"};
    CompressedEntry ct{1};

    entries.push_back(t);

    Dictionary dictionary(entries);
 
    EXPECT_EQ(dictionary.size(),1);
    EXPECT_EQ(dictionary.compress(t),ct);
}

TEST(DECTests, SingleEntryDecompressionTest) {
    std::vector<Entry> entries;
    Entry t{"aaa"};
    CompressedEntry ct{1};

    entries.push_back(t);

    Dictionary dictionary(entries);

    EXPECT_EQ(dictionary.size(),1);
    EXPECT_EQ(dictionary.decompress(ct),t);
}

TEST(DECTests, DuplicateEntryCompressionTest) {
    std::vector<Entry> entries;
    Entry t{"Hello"};
    CompressedEntry ct{1};

    for(int i=0;i<5;i++){
      entries.push_back(t);
    }

    Dictionary dictionary(entries);

    EXPECT_EQ(dictionary.size(),1);
    EXPECT_EQ(dictionary.compress(t),ct);
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

