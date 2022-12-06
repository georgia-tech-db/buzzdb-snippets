#pragma once
#include <vector>
#include <cstdint>

namespace buzzdb {
    struct SlottedPageScheme {
        struct Tuple {
            Tuple() = default;
            uint64_t data;
        };
        struct Header {
            Header(uint64_t page_size);
            uint16_t num_slots;
            uint16_t free_space;
            uint16_t page_size;
        };
        
        SlottedPageScheme(uint32_t page_size);
        void insertTuple(uint32_t value);
        void deleteTuple(uint16_t index);
        uint32_t find(uint16_t index);
        Header header;
        std::vector<Tuple> slot_array;
        ~SlottedPageScheme(){}
    };
}