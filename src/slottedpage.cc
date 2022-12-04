
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <deque>
#include "slottedpage.h"

namespace buzzdb {


SlottedPage::SlottedPage() : next_slot_id(0), page_size(sizeof(Header)), free_bytes(MAX_PAGE_SIZE - sizeof(Header)) {
    slots.clear();
    slots.shrink_to_fit();
    records.clear();
    records.shrink_to_fit();
    next_free_slot = INVALID_SLOT_ID;
    num_records = 0;
    header.slot_count = 0;
}
void SlottedPage::print_records() {
    std::cout << "============== RECORDS ==============" << std::endl;
    for (uint16_t i = 0; i < records.size(); i++) {
        std::cout << i << ":\t" << records[i] << std::endl;
    }
    std::cout << "============== END RECORDS ==============" << std::endl;

}

void SlottedPage::print_slots() {
     std::cout << "============== SLOTS ==============" << std::endl;
    for (uint16_t i = 0; i < slots.size(); i++) {
        if (!slots[i].valid) continue;
        std::cout << "slot id:\t" << slots[i].slot_id << "\trecord_index:\t" << slots[i].record << "\tRecord:\t" << records[slots[i].record] << std::endl;
    }
    std::cout << "============== END SLOTS ==============" << std::endl;

}

void SlottedPage::update_record_indicies(bool add, uint16_t slot_id) {
    uint16_t record_start = slots[slot_id].record;
    if (add) {
        for (uint16_t i = 0; i < slots.size(); i++) {
            if (slots[i].record >= record_start && slots[i].slot_id != slot_id) {
                slots[i].record++;
            }
        }
    } else {
        for (uint16_t i = 0; i < slots.size(); i++) {
            if (slots[i].record >= record_start && slots[i].slot_id != slot_id) {
                slots[i].record--;
            }
        }
    }
}
uint32_t SlottedPage::insertRecord(Record &r) {
    
    
    //std::cout << "============== INSERT ==============" << std::endl;
    auto slotpair = find_free_slot(r.data.length());
    uint16_t id = slotpair.first;
    bool found_free_slot = slotpair.second.first;
    bool slot_is_large_enough = slotpair.second.second;
    uint16_t bytes_removed;


    slots[id].length = r.data.length();
    free_bytes -= slots[id].length;
    slots[id].valid = true;

    if (found_free_slot && slot_is_large_enough) {
        if (slots[id].record == 0) {
            bytes_removed = std::string(records.front()).length();
            records.pop_front();
            records.push_front(r.data.c_str());
        } else {
            bytes_removed = std::string(records[slots[id].record]).length();
            records.erase(records.begin() + slots[id].record);
            records.insert(records.begin() + slots[id].record, r.data.c_str());
        }
    } else if (found_free_slot && !slot_is_large_enough) {
        if (slots[id].record == 0) {
            bytes_removed = std::string(records.front()).length();
            records.pop_front();
            records.push_front(r.data.c_str());
        } else {
            bytes_removed = std::string(records[slots[id].record]).length();
            records.erase(records.begin() + slots[id].record);
            update_record_indicies(false, id);
            records.push_front(r.data.c_str());
            slots[id].record = 0;
            update_record_indicies(true, id);
        }
        
    } else {
        bytes_removed = 0;
        records.push_front(r.data.c_str());
        slots[id].record = 0;
        update_record_indicies(true, id);
    }

    header.slot_count++;
    num_records++;

    page_size -= bytes_removed;
    page_size += found_free_slot ? slots[id].length : sizeof(Slot) + slots[id].length;
    
    header.LSN = LSN_OP::INSERT;
    assert(slots[id].valid && "What the fuck");
    return slots[id].slot_id;
}

void SlottedPage::removeRecord(uint16_t slot_id) {
    //std::cout << "============== REMOVE ==============" << std::endl;
    assert(slots[slot_id].valid && "ATTEMPTED TO DELETE INVALID SLOT");

    slots[slot_id].valid = false;
    free_bytes += slots[slot_id].length;
    num_records--;
    header.LSN = LSN_OP::DELETE;
    header.slot_count--;
   
}

void SlottedPage::update_record(uint16_t slot_id, Record &new_record) {
    assert(slots[slot_id].valid && "Attempted to update invalid record");

    if (slots[slot_id].length >= new_record.data.length()) {
        
        page_size -= slots[slot_id].length;
        free_bytes += slots[slot_id].length; 
        
        if (slots[slot_id].record == records.size() - 1) {
            records.pop_back();
            records.push_back(new_record.data.c_str());
        } else {
            records.erase(records.begin() + slots[slot_id].record);
            records.insert(records.begin() + slots[slot_id].record, new_record.data.c_str());
            slots[slot_id].length = new_record.data.length();
        }

        page_size += new_record.data.length();
        free_bytes -= new_record.data.length();
        header.LSN = LSN_OP::UPDATE;
        return;
    } else {
        assert(page_size - slots[slot_id].length + new_record.data.length() <= MAX_PAGE_SIZE && "Cannot insert record, TOO LARGE FOR PAGE");

        page_size -= slots[slot_id].length;
        free_bytes += slots[slot_id].length;

        if (slots[slot_id].record == records.size() - 1) {
            records.pop_back();
        } else {
            records.erase(records.begin() + slots[slot_id].record);
            update_record_indicies(false, slot_id);
        }
        
        records.push_front(new_record.data.c_str());
        slots[slot_id].length = new_record.data.length();
        slots[slot_id].record = 0;
        update_record_indicies(true, slot_id);

        page_size += new_record.data.length();
        free_bytes -= new_record.data.length();
        header.LSN = LSN_OP::UPDATE;
        return;
    }

    assert(false && "Unable to insert record, requires compactificaton. Toy example does not handle");

}

// slot, (slot exists?, is there space)
std::pair<uint16_t, std::pair<bool, bool>>SlottedPage::find_free_slot(size_t min_size) {
    std::pair<uint16_t, std::pair<bool, bool>> best_effort(0, std::pair<bool, bool>(false, false));
    for (uint16_t i = 0; i < slots.size(); i++) {
        assert(slots[i].slot_id == i && "SLOT IDS DONT MATCH");
        if (slots[i].valid)
            continue;
        if (slots[i].length >= min_size)
            return std::pair<uint16_t, std::pair<bool, bool>>(i, std::pair<bool, bool>(true, true));
        if (slots[i].length < min_size && free_bytes > min_size) 
            best_effort = std::pair<uint16_t, std::pair<bool, bool>>(i, std::pair<bool, bool>(true, false));
    }
    if (best_effort.second.first)
        return best_effort;
    slots.push_back(Slot(next_slot_id++));
    return std::pair<uint16_t, std::pair<bool, bool>> (next_slot_id - 1, std::pair<bool, bool>(false, false));
}

}