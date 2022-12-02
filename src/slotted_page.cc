#include "utils/slotted_page.h"
#include <cstdint>

using SlottedPage = buzzdb::SlottedPage;
using SlotReference = buzzdb::SlotReference;

SlotReference::SlotReference(uint64_t page, uint16_t slot) {
    this -> slot_id_value = (page << 16) | slot;
}

SlottedPage::Header::Header(uint32_t page_size) {
    this -> slot_count = 0;
    // Header data takes up around 4 bytes of space.
    this -> next_free_slot = page_size - 4;
    this -> page_size = page_size;
}

SlottedPage::SlottedPage(uint32_t page_size) : slot_header(page_size) {
    this -> data.resize(page_size - 4);
}

uint16_t SlottedPage::addSlot(uint32_t stored_value) {
    // Case where we have a free slot somewhere and therefore it would be
    // wasteful to add a new slot.
    if (this -> slot_header.next_free_slot != this -> slot_header.page_size - 4) {
        this -> slots[this -> slot_header.next_free_slot].stored_value = stored_value;
        uint16_t old_free_slot = this -> slot_header.next_free_slot;
        int index = 0;

        for (auto slot : slots) {
            if (slot.stored_value == (uint32_t) -1) {
                this -> slot_header.next_free_slot = index;
                break;
            }
            index++;
        }

        if (this -> slot_header.next_free_slot == old_free_slot) {
            this -> slot_header.next_free_slot = -1;
        }

        return old_free_slot;
    } else {
        Slot new_slot;
        new_slot.stored_value = stored_value;
        this -> slot_header.slot_count++;
        this -> slots.push_back(new_slot);
        return this -> slot_header.slot_count;
    }  
}

uint16_t SlottedPage::removeSlot(int index) {
    this -> slots.erase(this -> slots.begin() + index);
    this -> slot_header.slot_count--;
    return this -> slot_header.slot_count;
}

uint16_t SlottedPage::removeSlot(uint32_t value_to_remove) {
    int index = 0;

    for (auto slot : this -> slots) {
        if (slot.stored_value == value_to_remove) {
            this -> slots[index].stored_value = -1;
            if (this -> slot_header.next_free_slot == (uint16_t) -1 || 
                (this -> slot_header.next_free_slot != (uint16_t) -1 && index < this -> slot_header.next_free_slot)) {
                this -> slot_header.next_free_slot = index;
            }
            break;
        }
        index++;
    }

    return index;
}