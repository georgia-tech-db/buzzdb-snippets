

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

TEST(DECTests, EmptyDictionaryAccessExceptionTest) {
    std::vector<Entry> entries;
    Dictionary dictionary(entries);
    EXPECT_EQ(dictionary.size(),0);
    EXPECT_THROW(dictionary.compress(Entry{"Mozart"}), std::out_of_range);
}

TEST(DECTests, SingleEntryCompressionTest) {
    std::vector<Entry> entries;
    Entry t{"Mozart"};
    CompressedEntry ct{1};

    entries.push_back(t);

    Dictionary dictionary(entries);
 
    EXPECT_EQ(dictionary.size(),1);
    EXPECT_EQ(dictionary.compress(t),ct);
}

TEST(DECTests, SingleEntryDecompressionTest) {
    std::vector<Entry> entries;
    Entry t{"Mozart"};
    CompressedEntry ct{1};

    entries.push_back(t);

    Dictionary dictionary(entries);

    EXPECT_EQ(dictionary.size(),1);
    EXPECT_EQ(dictionary.decompress(ct),t);
}

TEST(DECTests, DuplicateEntryCompressionTest) {
    std::vector<Entry> entries;
    Entry t{"Mozart"};
    CompressedEntry ct{1};

    for(int i=0;i<5;i++){
      entries.push_back(t);
    }

    Dictionary dictionary(entries);

    EXPECT_EQ(dictionary.size(),1);
    EXPECT_EQ(dictionary.compress(t),ct);
}

TEST(DECTests, MultipleEntryInsertionTest) {
    std::vector<Entry> entries;
    
    entries.push_back(Entry{"Mozart"});
    entries.push_back(Entry{"Beethoven"});
    entries.push_back(Entry{"Mozart"});
    entries.push_back(Entry{"Bach"});

    Dictionary dictionary(entries);

    EXPECT_EQ(dictionary.size(),3);
}

TEST(DECTests, CompressionDecompressionTest) {
    std::vector<Entry> entries;
    
    entries.push_back(Entry{"Mozart"});
    entries.push_back(Entry{"Beethoven"});
    entries.push_back(Entry{"Mozart"});
    entries.push_back(Entry{"Bach"});

    Dictionary dictionary(entries);

    EXPECT_EQ(dictionary.compress(Entry{"Mozart"}),CompressedEntry{1});
    EXPECT_EQ(dictionary.compress(Entry{"Beethoven"}),CompressedEntry{2});
    EXPECT_EQ(dictionary.compress(Entry{"Bach"}),CompressedEntry{3});

    EXPECT_EQ(dictionary.decompress(CompressedEntry{1}),Entry{"Mozart"});
    EXPECT_EQ(dictionary.decompress(CompressedEntry{2}),Entry{"Beethoven"});
    EXPECT_EQ(dictionary.decompress(CompressedEntry{3}),Entry{"Bach"});
}


}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

