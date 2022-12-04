#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>

#include "slottedpage.h"
namespace {

std::vector<buzzdb::Record> records_to_insert = {
    buzzdb::Record("i am record 0"), 
    buzzdb::Record("i am record 1"), 
    buzzdb::Record("this is another record woo"),
    buzzdb::Record("anotha1"),
    buzzdb::Record("this is a record for a longer length"),
    buzzdb::Record("smol"),
    buzzdb::Record("HUGE NORMOUS RECORD OMG I HAVE NO WRDS THIS IS VERY VERY LONG AND VARIABLE"),
    buzzdb::Record("another variable rec"),
  };

TEST(SlottedPageTests, BasicTest) {

    buzzdb::SlottedPage page;
   
    EXPECT_EQ (page.page_size == sizeof(buzzdb::Header), true);
    uint32_t record0 = page.insertRecord(records_to_insert[0]);
    EXPECT_EQ (page.page_size == sizeof(buzzdb::Header) + sizeof(buzzdb::Slot) + records_to_insert[0].data.length(), true);
    uint32_t record1 = page.insertRecord(records_to_insert[1]);
    EXPECT_EQ(page.header.slot_count == 2 ,true);
    EXPECT_EQ(record0 == record1, false);
}

TEST(SlottedPageTests, RemoveRecord) {

    buzzdb::SlottedPage page;    
    uint64_t pagesize = sizeof(buzzdb::Header);

    EXPECT_EQ (page.page_size == pagesize, true);
    page.insertRecord(records_to_insert[0]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[0].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);
    
    uint32_t record1 = page.insertRecord(records_to_insert[1]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[1].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);
    EXPECT_EQ (record1 == 1, true);

    page.insertRecord(records_to_insert[2]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[2].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);

    page.removeRecord(record1);
    EXPECT_EQ (page.page_size == pagesize, true); // pagesize doesnt change
    EXPECT_EQ (page.header.slot_count == 2, true);
}

TEST(SlottedPageTests, InsertVariableRecords) {

    buzzdb::SlottedPage page;    
    uint64_t pagesize = sizeof(buzzdb::Header);
    

    EXPECT_EQ (page.page_size == pagesize, true);
    page.insertRecord(records_to_insert[0]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[0].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);
    
    page.insertRecord(records_to_insert[1]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[1].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);

    page.insertRecord(records_to_insert[2]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[2].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);
}

TEST(SlottedPageTests, FillPage) {

    buzzdb::SlottedPage page;
    std::vector<buzzdb::Record> records_to_insert;
    std::vector<uint32_t> record_tids;
    
    uint64_t pagesize = sizeof(buzzdb::Header);
    for (int i = 0; i < 220; i++) {
      records_to_insert.push_back(buzzdb::Record("i am record " + i));
    }
    
    for (auto rec: records_to_insert) {
      uint32_t tid;
      if ((tid = page.insertRecord(rec)) == INVALID_TID) {
        break;
      }
      record_tids.push_back(tid);
      pagesize += sizeof(buzzdb::Slot) + rec.data.length();
      EXPECT_EQ (page.page_size == pagesize, true);
    }
  
}

TEST(SlottedPageTests, VerifySlotReuse) {

    buzzdb::SlottedPage page;
    
    uint64_t pagesize = sizeof(buzzdb::Header);
  

    EXPECT_EQ (page.page_size == pagesize, true);
    page.insertRecord(records_to_insert[0]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[0].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);
    
    uint32_t record1 = page.insertRecord(records_to_insert[1]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[1].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);

    page.insertRecord(records_to_insert[2]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[2].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);

    page.removeRecord(record1);
    EXPECT_EQ (page.page_size == pagesize, true); // pagesize doesnt change

    uint32_t record4 = page.insertRecord(records_to_insert[0]); // should reuse deleted slot
    EXPECT_EQ (record1 == record4, true);
    EXPECT_EQ (page.header.slot_count == 3, true);
}

TEST(SlottedPageTests, UpdateRecordSmaller) {

    buzzdb::SlottedPage page;
    
    uint64_t pagesize = sizeof(buzzdb::Header);
  
    EXPECT_EQ (page.page_size == pagesize, true);
    uint16_t record0 = page.insertRecord(records_to_insert[0]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[0].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);

    page.update_record(record0, records_to_insert[2]);
    pagesize = pagesize - records_to_insert[0].data.length() + records_to_insert[2].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);
}

TEST(SlottedPageTests, UpdateRecordLarger) {

    buzzdb::SlottedPage page;
    
    
    uint64_t pagesize = sizeof(buzzdb::Header);
    
    

    EXPECT_EQ (page.page_size == pagesize, true);
    uint16_t record0 = page.insertRecord(records_to_insert[0]);
    pagesize += sizeof(buzzdb::Slot) + records_to_insert[0].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);

    page.update_record(record0, records_to_insert[1]);
    pagesize = pagesize - records_to_insert[0].data.length() + records_to_insert[1].data.length();
    EXPECT_EQ (page.page_size == pagesize, true);
}

TEST(SlottedPageTests, TestAll) {

    buzzdb::SlottedPage page;
    uint64_t pagesize = sizeof(buzzdb::Header);

    std::vector<uint16_t> tuples;
    std::vector<uint16_t> backwards_tuples;

    for (uint32_t i = 0; i < records_to_insert.size(); i++) { // insert all records
      tuples.push_back(page.insertRecord(records_to_insert[i]));
      pagesize += sizeof(buzzdb::Slot) + records_to_insert[i].data.length();
      EXPECT_EQ (page.page_size == pagesize, true);
    }

    for (uint16_t i = 0; i < tuples.size(); i++) { // remove all records
      page.removeRecord(tuples[i]);
      EXPECT_EQ (page.page_size == pagesize, true);
    }


    for (int i = records_to_insert.size() - 1; i >= 0; i--) {
      backwards_tuples.push_back(page.insertRecord(records_to_insert[i]));
    }

   
    buzzdb::Record a("a");
    for (uint16_t i = 0; i < backwards_tuples.size(); i++) {
      page.update_record(backwards_tuples[i], a);
    }
    EXPECT_EQ (page.page_size == sizeof(buzzdb::Header) + (backwards_tuples.size() * sizeof(buzzdb::Slot)) + backwards_tuples.size(), true);
    
} 
}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
