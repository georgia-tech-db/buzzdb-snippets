#include "utils/slottedPageScheme.h"
#include <iostream>
#include <cstdint>
#include <vector>

using SlottedPageScheme = buzzdb::SlottedPageScheme;

SlottedPageScheme::Header::Header(uint64_t page_size) {
    this->page_size = page_size;
    this->num_slots = slot_array.size();
    this->free_space = page_size - sizeof(header);
}

SlottedPageScheme::SlottedPageScheme(uint64_t page_size) : header(page_size) {
    this->header = Header(page_size);
    std::cout << "Slotted Page Created with header of size: " << sizeof(header) << 
    " and free space of size: " << (page_size - sizeof(header)) << " \n";
}

void SlottedPageScheme::insertTuple(uint64_t data) {
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
    if (index >= 0 && index < header.num_slots) {
        uint64_t size = sizeof(slot_array[index]);
        slot_array.erase(index + slot_array.begin());
        header.free_space += size;
        header.num_slots--;
    }
}

uint64_t SlottedPageScheme::find(uint16_t index) {
    if (index >= 0 && index < header.num_slots) {
        std::cout << "Found data in slotted page\n";
        return slot_array[index].data;
    } else {
        std::cout << "Not a valid index\n";
        return -1;
    }
}