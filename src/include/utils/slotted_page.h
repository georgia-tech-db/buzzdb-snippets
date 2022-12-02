#pragma once

#include <vector>
#include <cstdint>

namespace buzzdb {

struct SlottedPage {
    struct Header {
        Header(uint32_t page_size);

        uint16_t slot_count;
        uint16_t next_free_slot;
        uint32_t page_size;
    };

    struct Slot {
        Slot() = default;

        uint32_t stored_value;
    };

    SlottedPage(uint32_t page_size);

    uint16_t addSlot(uint32_t stored_value);

    // Completely remove a slot at a particular index.
    uint16_t removeSlot(int index);

    // Removes the first instance of a value found in a slot without removing the
    // slot completely.
    uint16_t removeSlot(uint32_t value_to_remove);

    Header slot_header;
    std::vector<std::byte> data;
    std::vector<Slot> slots;
};

struct SlotReference {
    SlotReference(uint64_t page, uint16_t slot);

    // 48 bytes page id and 16 bytes for slot id = 64 byte slot id value
    uint64_t slot_id_value;
};

}