#include "utils/slottedPage.h"
#include <iostream>
#include <cstdint>
#include <vector>

using SlottedPage = buzzdb::SlottedPage;


SlottedPage::Header::Header(uint32_t page_size) {
    this->slot_count = 0;
    this->first_free_slot = page_size - sizeof(header);
    this->page_size = page_size;
    this->free_space = page_size - sizeof(header);
}

SlottedPage::SlottedPage(uint32_t page_size) : header(page_size) {
    this->header = Header(page_size);
}

void SlottedPage::insertRecord(uint32_t value) {
    
    //std::cout << header.first_free_slot << "first_free_slot\n";
    
    Slot new_slot;
    
    new_slot.value = value;
    //std::cout << sizeof(new_slot) << "SLOT\n";
    // auto it = slots.begin();
    // slots.insert(it + (header.first_free_slot / sizeof(new_slot)), new_slot);
    // header.first_free_slot = header.first_free_slot - sizeof(value);
    //insert to new position
    if (header.first_free_slot == header.free_space) {

        slots.push_back(new_slot);
        header.slot_count++;
        header.free_space -= sizeof(new_slot);
        header.first_free_slot -= sizeof(new_slot);
    } else {
        std::cout << header.first_free_slot << "first_free_slot\n";
        
        
        slots[header.first_free_slot].value = value;
    }

}

void SlottedPage::deleteRecord(uint16_t index) {
    //std::cout << header.first_free_slot << "index\n";
    //leave space if removed index is mid index. If it's the end, erase it
    slots[index].value = 1111;
    header.first_free_slot = index;
    if (index == slots.size() - 1) {
        std::cout << slots.size() << "sizeof\n";
        slots.erase(slots.begin() + index);
        header.free_space += 4;
        header.slot_count--;
    }
    //}
}

uint32_t SlottedPage::lookUpRecord(uint16_t index) {
    return slots[index].value;
}
