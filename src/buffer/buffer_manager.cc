
#include <cassert>
#include <iostream>
#include <string>
#include <limits.h>
#include <memory>

#include "buffer/buffer_manager.h"
#include "common/macros.h"
#include "storage/file.h"

#define UNUSED(p)  ((void)(p))

namespace buzzdb {

char* BufferFrame::get_data() {
    return reinterpret_cast<char*>(data.data());
}

BufferFrame::BufferFrame(size_t page_size) {
    this->data = std::vector<uint64_t>(page_size);
    this->count = 0;
    this->dirty = false;
}

BufferManager::BufferManager(size_t page_size, size_t page_count) {
    this->page_size = page_size;
    this->page_count = page_count;
    for(size_t i=0; i<page_count; i++){
      pool_.push_back(std::make_unique<BufferFrame>(page_size));
    }
}



BufferManager::~BufferManager() {
    for (size_t i = 0; i < pool_.size(); i++)
    {
        if(pool_[i]->dirty){
            flush(i);
        }
    } 
}

std::pair<bool, uint64_t> BufferManager::page_in_fifo_queue(uint64_t page_id){

    bool found_page = false;
    uint64_t page_frame_id = INT_MAX;
    for(size_t i = 0; i < fifo_queue_.size(); i++){
        auto frame_id = fifo_queue_[i];
        if(pool_[frame_id]->page_id==page_id){
            found_page=true;
            page_frame_id=frame_id;
            pool_[page_frame_id]->count++;
            fifo_queue_.erase(fifo_queue_.begin() + i);
            lru_queue_.push_back(frame_id);
            break;
        }
    }
    return std::make_pair(found_page, page_frame_id);
}

//find page in lru queue
std::pair<bool, uint64_t> BufferManager::page_in_lru_queue(uint64_t page_id){
    bool found_page = false;
    uint64_t page_frame_id = INT_MAX;
    for(size_t i = 0; i < lru_queue_.size(); i++){
        auto frame_id = lru_queue_[i];
        if(pool_[frame_id]->page_id==page_id){
            found_page=true;
            page_frame_id=frame_id;
            pool_[page_frame_id]->count++;
            lru_queue_.erase(lru_queue_.begin() + i);
            lru_queue_.push_back(frame_id);
            break;
        }
    }
    return std::make_pair(found_page, page_frame_id);
}

// read page to data
void BufferManager::read_frame(uint64_t frame_id){
    file_use_mutex_.lock();
    int16_t segment_id = BufferManager::get_segment_id(pool_[frame_id]->page_id);
    uint64_t segment_page_id = BufferManager::get_segment_page_id(pool_[frame_id]->page_id);
    auto file_handle = File::open_file(std::to_string(segment_id).c_str(), File::WRITE);
    if(file_handle->size() >= segment_page_id * page_size + page_size){
        file_handle->read_block(segment_page_id * page_size, page_size, pool_[frame_id]->get_data());
    }
    file_use_mutex_.unlock();
}

// write frame to disk
void BufferManager::flush(uint64_t frame_id){
    file_use_mutex_.lock();
    uint16_t segment_id = BufferManager::get_segment_id(pool_[frame_id]->page_id);
    uint64_t segment_page_id = BufferManager::get_segment_page_id(pool_[frame_id]->page_id);
    auto file_handle = File::open_file(std::to_string(segment_id).c_str(), File::WRITE);
    file_handle->write_block(pool_[frame_id]->get_data(), segment_page_id * page_size, page_size);
    file_use_mutex_.unlock();
}

void BufferManager::lock_frame(uint64_t frame_id, bool exclusive){
    // std::cout << "current frame count in lock: "<< pool_[frame_id]->count.load() << std::endl;
    if(!exclusive){
        pool_[frame_id]->mutex_.lock_shared();
    } else{
        pool_[frame_id]->mutex_.lock();
        pool_[frame_id]->exclusive_thread_id = std::this_thread::get_id();
    }
    pool_[frame_id]->exclusive = exclusive;
}

void BufferManager::unlock_frame(uint64_t frame_id){
    
    pool_[frame_id]->count--;
    // std::cout << "current frame count in unlock: "<< pool_[frame_id]->count << std::endl;
    if(!pool_[frame_id]->exclusive){
        pool_[frame_id]->mutex_.unlock_shared();
    } else{
        pool_[frame_id]->mutex_.unlock();
        pool_[frame_id]->exclusive = false;
    }
}

BufferFrame& BufferManager::fix_page(uint64_t page_id, bool exclusive) {
    fifo_mutex_.lock();
    lru_mutex_.lock();
    std::cout << "page_id " <<  page_id<< " this_thread_id before lock: " <<  std::this_thread::get_id() << std::endl;
    // find in lru queue
    std::pair<bool, uint64_t> lru_result = page_in_lru_queue(page_id);
    if(lru_result.first){
        // std::cout << "lru result count after unlock: " <<   pool_[lru_result.second]->count << std::endl;
        fifo_mutex_.unlock();
        lru_mutex_.unlock();
        lock_frame(lru_result.second, exclusive);
        return *pool_[lru_result.second];
    }
    // find in fifo queue
    std::pair<bool, uint64_t> fifo_result = page_in_fifo_queue(page_id);
    if(fifo_result.first){
        // std::cout << "fifo after unlock: " <<   pool_[fifo_result.second]->count << std::endl;
        fifo_mutex_.unlock();
        lru_mutex_.unlock();
        lock_frame(fifo_result.second, exclusive);
        return *pool_[fifo_result.second];
    }

    // find a free slot
    uint64_t page_frame_id = 0;
    bool found_page = false;
    // buffer is full
    if(buffer_size == page_count){
        // evict unfixed page from fifo queue
        for(size_t i = 0; i < fifo_queue_.size(); i++){
            auto frame_id = fifo_queue_[i];
            // std::cout << "current frame count fifo: "<< pool_[frame_id]->count << std::endl;
            if(pool_[frame_id]->count == 0){
                found_page=true;
                page_frame_id=frame_id;
                if(pool_[frame_id]->dirty){
                    flush(frame_id);
                }
                fifo_queue_.erase(fifo_queue_.begin() + i);
                break;
            }
        }
        if(!found_page){
            // evict unfixed page from lru queue
            for(size_t i = 0; i < lru_queue_.size(); i++){
                auto frame_id = lru_queue_[i];
                // std::cout << "current frame count lru: "<< pool_[frame_id]->count << std::endl;
                if(pool_[frame_id]->count == 0){
                    found_page=true;
                    page_frame_id=frame_id;
                    if(pool_[frame_id]->dirty){
                        flush(frame_id);
                    }
                    lru_queue_.erase(lru_queue_.begin() + i);
                    break;
                }
            }
        }
        if(!found_page){
            fifo_mutex_.unlock();
            lru_mutex_.unlock();
            throw buffer_full_error{}; 
        }
    } else{
        page_frame_id = buffer_size;
        buffer_size.fetch_add(1);
    }

    lock_frame(page_frame_id, true);
    pool_[page_frame_id]->page_id = page_id;
    pool_[page_frame_id]->frame_id = page_frame_id;
    pool_[page_frame_id]->dirty = false;
    pool_[page_frame_id]->count = 2;
    fifo_queue_.push_back(page_frame_id);
    fifo_mutex_.unlock();
    lru_mutex_.unlock();
    // std::cout << "this_thread_id after unlock: " <<  std::this_thread::get_id() << std::endl;
    read_frame(page_frame_id);
    // std::cout << "this_thread_id after unlock: " <<  std::this_thread::get_id() << std::endl;
    unlock_frame(page_frame_id);
    // std::cout << "this_thread_id after unlock: " <<  std::this_thread::get_id() << std::endl;
    lock_frame(page_frame_id, exclusive);
    // std::cout << "this_thread_id after unlock: " <<  std::this_thread::get_id() << std::endl;

    return *pool_[page_frame_id];
}



void BufferManager::unfix_page(BufferFrame& page, bool is_dirty) {
    if(!page.dirty){
        page.dirty = is_dirty;
    }
    for(uint64_t i = 0; i < pool_.size(); i++){
        if(pool_[i].get() == &page){
            unlock_frame(i);
        }
    }
}


std::vector<uint64_t> BufferManager::get_fifo_list() const {
    std::vector<uint64_t> ans(fifo_queue_.size());
    for(size_t i = 0; i < fifo_queue_.size(); i++){
        ans[i] = pool_[fifo_queue_[i]]->page_id;
    }
    return ans;
}


std::vector<uint64_t> BufferManager::get_lru_list() const {
    std::vector<uint64_t> ans(lru_queue_.size());
    for(size_t i = 0; i < lru_queue_.size(); i++){
        ans[i] = pool_[lru_queue_[i]]->page_id;
    }
    return ans;
}

}  // namespace buzzdb
