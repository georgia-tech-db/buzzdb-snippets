#pragma once

#include <string>
#include <map>
#include <deque>

#define MAX_PAGE_SIZE 5120
#define INVALID_TID 11111
#define INVALID_SLOT_ID 11111

namespace buzzdb {


struct Record {
    std::string data;
    Record(std::string data) : data(data) {}
};


struct Slot {
    uint16_t slot_id = INVALID_SLOT_ID;
    uint64_t length; // length of the record
    bool valid = false;
    uint16_t record;

    Slot(uint16_t slot_id) : slot_id(slot_id), length(0) {}

} ;



enum LSN_OP {UPDATE, DELETE, INSERT};
struct Header {
    LSN_OP LSN; //lock sequence number (previous operation)
    uint32_t slot_count = 0; // slot count
    uint16_t first_free_slot = 0; // first free slot, index in slot vector
    Record *data_start; // address of lower data
    uint16_t free_space = 0; // free space for compaction check


    Header() {}
};




class SlottedPage {

public:
    Header header;
    std::vector<Slot> slots;
    std::deque<const char *> records;

    uint16_t next_slot_id;
    uint16_t next_free_slot; // index in array
    uint64_t page_size;
    uint64_t free_bytes;
    uint16_t num_records; 


    SlottedPage();

    // returns the slot id created 
    uint32_t insertRecord(Record& r);
	void removeRecord(uint16_t slotId);
    void update_record(uint16_t slotId, Record &new_record);
    void update_record_indicies(bool add, uint16_t slot_id);
    std::pair<uint16_t, std::pair<bool, bool>> find_free_slot(size_t min_size);
	void print_slots();
    void print_records();
};

}