#include <cassert>
#include <iostream>
#include <string>

#include "utils/buffermanager.h"
#include "utils/storage/file.h"

namespace buzzdb {

BufferFrame::BufferFrame(uint64_t page_number, uint64_t frame_size, vector<uint64_t> data){
    this->page_number = page_number;
    this->dirty_bit = false;
    this->counter = 1;
    this->data = data;
    if (this->data.size() == 0) {
        this->data.resize(frame_size / sizeof(uint64_t));
    }
    this->exclusive = true;
}

char* BufferFrame::get_data() {

    auto data = reinterpret_cast<char *>(this->data.data());
    return data;
}


BufferManager::BufferManager(size_t page_size, size_t page_count) {
    this->page_size = page_size;
    this->page_count = page_count;
}


BufferManager::~BufferManager() {

    for (BufferFrame frame : fifo_queue){
        if(frame.dirty_bit == true){
            string file_name = to_string(BufferManager::get_segment_id(frame.page_number));
            const char* filename = file_name.c_str();
            auto file = File::open_file(filename, File::WRITE);
            uint64_t offSet = get_segment_page_id(frame.page_number) * page_size;
            file->write_block(reinterpret_cast<char *>(frame.data.data()), offSet, page_size);
        }
    }

}


BufferFrame& BufferManager::fix_page(uint64_t page_id, bool exclusive) {

    //Exists in fifo 
    if (fifo_map.count(page_id) != 0){

        int place_in_queue = fifo_map[page_id];

        BufferFrame current_frame = fifo_queue[place_in_queue];
        current_frame.counter += 1;
        current_frame.exclusive = exclusive;
        fifo_map.clear();

        return fifo_queue[place_in_queue];
    }
    //does not exist fifo and we have memory left
    else if (fifo_queue.size() < page_count){
        vector<uint64_t> data;
        data.resize(page_size/sizeof(uint64_t));

        string file_name = to_string(BufferManager::get_segment_id(page_id));
        const char* filename = file_name.c_str();
        auto file = File::open_file(filename, File::WRITE);
        uint64_t offSet = BufferManager::get_segment_page_id(page_id) * page_size;
        file->read_block(offSet, page_size, reinterpret_cast<char *>(data.data()));

        BufferFrame current_frame(page_id, this->page_size, data);
        current_frame.exclusive = exclusive;
        int new_position = fifo_queue.size();

        fifo_queue.push_back(current_frame);
        fifo_map[current_frame.page_number] = new_position;

        return fifo_queue.back();
    } 
    else {

        //check fifo for free slot aka counter = 0
        int index = -1;
        for (size_t i = 0; i < fifo_queue.size(); i++){
            if (fifo_queue[i].counter == 0){
                index = i;
                break;
            }
        }
        
        //if a counter = 0 was found to evict, we will evict it and add the page to fifo
        if (index != -1){

            BufferFrame frame_to_evict = fifo_queue[index];
            if (frame_to_evict.dirty_bit == true){
                string file_name = to_string(BufferManager::get_segment_id(frame_to_evict.page_number));
                const char* filename = file_name.c_str();
                auto file = File::open_file(filename, File::WRITE);
                uint64_t offset = get_segment_page_id(frame_to_evict.page_number) * page_size;
                file->write_block(reinterpret_cast<char *>(frame_to_evict.data.data()), offset, page_size);
            }
            fifo_queue.erase(fifo_queue.begin() + index);
            fifo_map.clear();
            for (size_t i = 0; i < fifo_queue.size(); i++){
                fifo_map[fifo_queue[i].page_number] = i;
            }
            //get data from disk and add it to fifo queue

            vector<uint64_t> data;
            data.resize(page_size/sizeof(uint64_t));
            string file_name = to_string(BufferManager::get_segment_id(page_id));
            const char* filename = file_name.c_str();
            auto file = File::open_file(filename, File::WRITE);
            uint64_t offSet = BufferManager::get_segment_page_id(page_id) * page_size;
            file->read_block(offSet, page_size, reinterpret_cast<char *>(data.data()));
            BufferFrame current_frame(page_id, this->page_size, data);
            current_frame.exclusive = exclusive;
            int new_position = fifo_queue.size();
            fifo_queue.push_back(current_frame);
            fifo_map[current_frame.page_number] = new_position;

            return fifo_queue.back();
        } else {

            throw buffer_full_error{};

        }
    } 
}


void BufferManager::unfix_page(BufferFrame& page, bool is_dirty) {

    if (fifo_map.count(page.page_number) > 0) {
        int index = fifo_map[page.page_number];
        fifo_queue[index].counter -= 1;
        fifo_queue[index].dirty_bit = is_dirty;
    }
}

std::vector<uint64_t> BufferManager::get_fifo_list() const {

    vector<uint64_t> id_numbers;
    for (BufferFrame frame: this->fifo_queue) {
        id_numbers.push_back(frame.page_number);
    }
    return id_numbers;
}
} 