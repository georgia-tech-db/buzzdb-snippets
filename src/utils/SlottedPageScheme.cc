#include "utils/slottedPageScheme.h"
#include <iostream>
#include <cstdint>
#include <vector>

using SlottedPageScheme = buzzdb::SlottedPageScheme;

SlottedPageScheme::Header::Header(uint32_t page_size) {
    this->page_size = page_size;
    this->num_slots = slot_array.size();
    //this->first_free_slot = page_size - sizeof(header);
    this->free_space = page_size - sizeof(header);
}

SlottedPageScheme::SlottedPage(uint32_t page_size) : header(page_size) {
    this->header = Header(page_size);
}

void SlottedPageScheme::insertTuple(uint32_t data) {
    Tuple tuple;
    tuple.data = data;
    if (header.free_space - sizeof(tuple) >= 0) {
        slot_array.push_back(tuple);
        header.free_space -= sizeof(tuple);
        header.num_slots = slot_array.size();
    } else {
        std::cout << "Not enough free space\n";
    }
}

void SlottedPageScheme::deleteTuple(uint16_t index) {
    
   
    if (index >= 0 && index < header.slot_count) {
        uint64_t size = sizeof(slots[index]);
        slot_array.erase(slot_array.begin() + index);
        header.free_space += size;
        header.num_slots--;
    }
}

uint32_t SlottedPageScheme::find(uint16_t index) {
    if (index >= 0 && index < header.num_slots) {
        return slot_array[index].data;
    } else {
        return 0;
    }
}