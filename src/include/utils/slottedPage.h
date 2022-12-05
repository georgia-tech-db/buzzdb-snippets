#pragma once
#include <vector>
#include <cstdint>

namespace buzzdb {
struct SlottedPage {
    struct Header {
        Header(uint32_t page_size);
        //# of used slots
        uint16_t slot_count;
        //first available slot
        uint16_t first_free_slot;

        //uint32_t data_start;
        //available space
        uint32_t free_space;
        uint32_t page_size;
    };
    struct Slot {
        Slot() = default;
        uint32_t value;
    };
    SlottedPage(uint32_t page_size);
    void insertRecord(uint32_t value);
    void deleteRecord(uint16_t index);
    uint32_t lookUpRecord(uint16_t index);
    Header header;
    std::vector<Slot> slots;
    ~SlottedPage(){}
};
}