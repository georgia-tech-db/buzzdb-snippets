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
        
        SlottedPageScheme(uint64_t page_size);

        std::vector<Tuple> slot_array;

        void insertTuple(uint64_t value);

        Header header;

        void deleteTuple(uint16_t index);

        uint64_t find(uint16_t index);

        

        

        ~SlottedPageScheme(){}
    };
}